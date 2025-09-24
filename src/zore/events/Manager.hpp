#pragma once

#include "zore/events/Event.hpp"

#include <typeindex>

namespace zore::event {

	class HandlerBase;

	//========================================================================
	//	Event Manager Class
	//========================================================================

	class Manager {
	public:
		static void Subscribe(std::type_index type, HandlerBase* handler);
		static void Unsubscribe(std::type_index type, HandlerBase* handler);

		template<typename EventType>
		static inline void Dispatch(const EventType& event) {
			static_assert(std::is_base_of<EventBase, EventType>::value, "EventType must derive from EventBase");
			Dispatch(typeid(EventType), event);
		}

	private:
		static void Dispatch(std::type_index type, const EventBase& event);
	};
}