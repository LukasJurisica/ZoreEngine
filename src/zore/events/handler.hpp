#pragma once

#include "zore/events/manager.hpp"
#include <functional>
#include <vector>

namespace zore::event {

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
		requires(std::is_base_of_v<EventBase, EventType>)
	class Handler : public HandlerBase {
	public:
		explicit Handler() { Manager::Subscribe(typeid(EventType), nullptr); }
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
		MultiHandler(int priority = 0) : m_priority(priority) { Manager::Subscribe(typeid(EventBase), nullptr); }
		MultiHandler(const MultiHandler&) = delete;
		MultiHandler& operator=(const MultiHandler&) = delete;
		~MultiHandler() { Clear(); }

		static MultiHandler& Get() {
			static MultiHandler instance;
			return instance;
		}

		template <typename EventType, typename ClassType>
			requires(std::is_base_of_v<EventBase, EventType>)
		inline void Register(bool(ClassType::* callback)(const EventType&), ClassType* obj) {
			Register(callback, obj, m_priority);
		}

		template <typename EventType, typename ClassType>
			requires(std::is_base_of_v<EventBase, EventType>)
		inline void Register(bool(ClassType::* callback)(const EventType&), ClassType* obj, int priority) {
			m_handlers.emplace_back(new Handler<EventType>(callback, obj, priority));
		}

		template <typename EventType>
			requires(std::is_base_of_v<EventBase, EventType>)
		inline void Register(bool(*callback)(const EventType&)) {
			Register(callback, m_priority);
		}

		template <typename EventType>
			requires(std::is_base_of_v<EventBase, EventType>)
		inline void Register(bool(*callback)(const EventType&), int priority) {
			m_handlers.emplace_back(new Handler<EventType>(callback, priority));
		}
		
		inline void Clear() {
			for (HandlerBase* handler : m_handlers)
				delete handler;
			m_handlers.clear();
		}

	private:
		std::vector<HandlerBase*> m_handlers;
		int m_priority;
	};
}