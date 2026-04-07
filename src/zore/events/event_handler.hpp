#pragma once

#include "zore/events/event_manager.hpp"
#include <functional>
#include <vector>

namespace zore::event {

	template<typename EventType>
	using Callback = std::function<bool(const EventType&)>;

	//========================================================================
	//	Event Handler Base
	//========================================================================

	class HandlerBase {
	public:
		virtual ~HandlerBase() = default;
		virtual bool Execute(const event::Base& e) const = 0;

	public:
		int m_priority = 0;
	};

	//========================================================================
	//	Event Handler
	//========================================================================

	template<event::Derivative E>
	class Handler : public HandlerBase {
	public:
		explicit Handler();
		template <typename C>
		explicit Handler(bool(C::* func)(const E&), C* obj, int priority = 0);
		explicit Handler(Callback<E> callback, int priority = 0);
		Handler(const Handler&) = delete;
		Handler& operator=(const Handler&) = delete;
		~Handler() { Unregister(); }

		template <typename C>
		inline void Register(bool(C::* func)(const E&), C* obj, int priority = 0);
		inline void Register(Callback<E> callback, int priority = 0);
		inline void Unregister();
		inline bool Execute(const event::Base& e) const override;

	private:
		Callback<E> m_callback;
	};


	//========================================================================
	//	Multi Event Handler
	//========================================================================

	class MultiHandler {
	public:
		MultiHandler(int priority = 0) : m_priority(priority) { Manager::Subscribe(typeid(event::Base), nullptr); }
		MultiHandler(const MultiHandler&) = delete;
		MultiHandler& operator=(const MultiHandler&) = delete;
		~MultiHandler() { Clear(); }

		static inline MultiHandler& Get();
		inline void Clear();
		template <event::Derivative E, typename C>
		inline void Register(bool(C::* callback)(const E&), C* obj);
		template <event::Derivative E, typename C>
		inline void Register(bool(C::* callback)(const E&), C* obj, int priority);
		template <event::Derivative E>
		inline void Register(bool(*callback)(const E&));
		template <event::Derivative E>
		inline void Register(bool(*callback)(const E&), int priority);

	private:
		std::vector<HandlerBase*> m_handlers;
		int m_priority;
	};
}

#include "zore/events/event_handler.inl"