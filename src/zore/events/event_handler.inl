#pragma once

#ifdef __INTELLISENSE__
#include "zore/events/event_handler.hpp"
#endif

namespace zore::event {

	//========================================================================
	//	Event Handler Class
	//========================================================================

	template <event::Derivative E>
	Handler<E>::Handler() {
		Manager::Subscribe(typeid(E), nullptr);
	}

	template<event::Derivative E>
	template<typename C>
	Handler<E>::Handler(bool(C::* func)(const E&), C* obj, int priority) {
		Register(func, obj, priority);
	}

	template<event::Derivative E>
	Handler<E>::Handler(Callback<E> callback, int priority) {
		Register(callback, priority);
	}

	template<event::Derivative E>
	template<typename C>
	void Handler<E>::Register(bool(C::* func)(const E&), C* obj, int priority) {
		Register(std::bind(func, obj, std::placeholders::_1), priority);
	}

	template<event::Derivative E>
	void Handler<E>::Register(Callback<E> callback, int priority) {
		m_priority = priority;
		if (m_callback)
			Manager::Unsubscribe(typeid(E), this);
		m_callback = callback;
		if (m_callback)
			Manager::Subscribe(typeid(E), this);
	}

	template<event::Derivative E>
	void Handler<E>::Unregister() {
		if (m_callback)
			Manager::Unsubscribe(typeid(E), this);
		m_callback = nullptr;
	}

	template<event::Derivative E>
	inline bool Handler<E>::Execute(const event::Base& e) const {
		return m_callback(static_cast<const E&>(e));
	}

	//========================================================================
	//	Multi Event Handler Class
	//========================================================================

	MultiHandler& MultiHandler::Get() {
		static MultiHandler instance;
		return instance;
	}

	void MultiHandler::Clear() {
		for (HandlerBase* handler : m_handlers)
			delete handler;
		m_handlers.clear();
	}

	template <event::Derivative E, typename C>
	void MultiHandler::Register(bool(C::* callback)(const E&), C* obj) {
		Register(callback, obj, m_priority);
	}

	template <event::Derivative E, typename C>
	void MultiHandler::Register(bool(C::* callback)(const E&), C* obj, int priority) {
		m_handlers.emplace_back(new Handler<E>(callback, obj, priority));
	}

	template <event::Derivative E>
	void MultiHandler::Register(bool(*callback)(const E&)) {
		Register(callback, m_priority);
	}

	template <event::Derivative E>
	void MultiHandler::Register(bool(*callback)(const E&), int priority) {
		m_handlers.emplace_back(new Handler<E>(callback, priority));
	}
}