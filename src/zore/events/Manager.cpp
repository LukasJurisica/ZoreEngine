#include "zore/events/Manager.hpp"
#include "zore/events/Handler.hpp"
#include "zore/Debug.hpp"

#include <algorithm>
#include <unordered_map>

namespace zore::event {

	//========================================================================
	//	Event Manager Class
	//========================================================================

	using EventHandlerMapType = std::unordered_map<std::type_index, std::vector<HandlerBase*>>;

	static EventHandlerMapType& EventHandlerMap() {
		static EventHandlerMapType s_event_handlers;
		return s_event_handlers;
	}

	static bool comparator(HandlerBase* a, HandlerBase* b) { return a->m_priority > b->m_priority; }

	void Manager::Subscribe(std::type_index type, HandlerBase* handler) {
		static EventHandlerMapType& event_handlers = EventHandlerMap();
		if (!handler)
			return;
		auto iter = event_handlers.find(type);
		if (iter != event_handlers.end()) {
			std::vector<HandlerBase*>& handlers = iter->second;
			handlers.insert(std::upper_bound(handlers.begin(), handlers.end(), handler, comparator), handler);
		}
		else {
			event_handlers[type] = { handler };
		}
	}

	void Manager::Unsubscribe(std::type_index type, HandlerBase* handler) {
		static EventHandlerMapType& event_handlers = EventHandlerMap();
		auto iter = event_handlers.find(type);
		if (iter != event_handlers.end()) {
			std::vector<HandlerBase*>& handlers = iter->second;
			handlers.erase(std::find(handlers.begin(), handlers.end(), handler));
		}
	}

	void Manager::Dispatch(std::type_index type, const EventBase& event) {
		static EventHandlerMapType& event_handlers = EventHandlerMap();
		auto iter = event_handlers.find(type);
		if (iter != event_handlers.end()) {
			for (HandlerBase* handler : iter->second) {
				if (handler->Execute(event)) {
					break;
				}
			}
		}
	}
}