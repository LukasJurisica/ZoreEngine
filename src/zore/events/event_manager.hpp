#pragma once

#include "zore/events/event.hpp"
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

		template<event::Derivative E>
		static inline void Dispatch(const E& event) { Dispatch(typeid(E), event); }

	private:
		static void Dispatch(std::type_index type, const event::Base& event);
	};
}