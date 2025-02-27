#pragma once

#include "zore/events/Manager.hpp"

#include <functional>
#include <vector>

namespace zore::Event {

	template<typename EventType>
	using Callback = std::function<bool(const EventType&)>;

	//========================================================================
	//	Event Handler Base Class
	//========================================================================

	class HandlerBase {
	public:
		virtual ~HandlerBase() = default;
		virtual bool Execute(const EventBase& e) const = 0;

	public:
		int m_priority;
	};

	//========================================================================
	//	Event Handler Class
	//========================================================================

	template<typename EventType>
	class Handler : public HandlerBase {
	public:
		static_assert(std::is_base_of<EventBase, EventType>::value, "EventType must derive from Event");

	public:
		explicit Handler() = default;
		template <typename ClassType>
		explicit Handler(bool(ClassType::* func)(const EventType&), ClassType* obj, int priority = 0) { Register(func, obj, priority); }
		explicit Handler(Callback<EventType> callback, int priority = 0 ) { Register(callback, priority); }
		Handler(const Handler&) = delete;
		Handler& operator=(const Handler&) = delete;
		~Handler() { Unregister(); }

		template <typename ClassType>
		void Register(bool(ClassType::* func)(const EventType&), ClassType* obj, int priority = 0) {
			Register(std::bind(func, obj, std::placeholders::_1), priority);
		}
		void Register(Callback<EventType> callback, int priority = 0) {
			m_priority = priority;
			if (m_callback)
				Manager::Unsubscribe(typeid(EventType), this);
			m_callback = callback;
			if (m_callback)
				Manager::Subscribe(typeid(EventType), this);
		}
		void Unregister() {
			if (m_callback)
				Manager::Unsubscribe(typeid(EventType), this);
			m_callback = nullptr;
		}

		inline bool Execute(const EventBase& e) const override { return m_callback(static_cast<const EventType&>(e)); }

	private:
		Callback<EventType> m_callback;
	};


	//========================================================================
	//	Multi Event Handler Class
	//========================================================================

	class MultiHandler {
	public:
		MultiHandler(int priority = 0) : m_priority(priority) {}
		MultiHandler(const MultiHandler&) = delete;
		MultiHandler& operator=(const MultiHandler&) = delete;
		~MultiHandler() { for (HandlerBase* handler : m_handlers) delete handler; }

		static MultiHandler& Get() {
			static MultiHandler instance;
			return instance;
		}

		template <typename EventType, typename ClassType>
		inline void Register(bool(ClassType::* callback)(const EventType&), ClassType* obj) {
			Register(callback, obj, m_priority);
		}

		template <typename EventType, typename ClassType>
		inline void Register(bool(ClassType::* callback)(const EventType&), ClassType* obj, int priority) {
			static_assert(std::is_base_of<EventBase, EventType>::value, "EventType must derive from Event");
			m_handlers.emplace_back(new Handler<EventType>(callback, obj, priority));
		}

		template <typename EventType>
		inline void Register(bool(*callback)(const EventType&)) {
			Register(callback, m_priority);
		}

		template <typename EventType>
		inline void Register(bool(*callback)(const EventType&), int priority) {
			static_assert(std::is_base_of<EventBase, EventType>::value, "EventType must derive from Event");
			m_handlers.emplace_back(new Handler<EventType>(callback, priority));
		}

	private:
		std::vector<HandlerBase*> m_handlers;
		int m_priority;
	};
}