#include "zore/events/Manager.hpp"
#include "zore/events/Handler.hpp"
#include "zore/Debug.hpp"

#include <unordered_map>
#include <algorithm>

namespace zore {

	//========================================================================
	//	Event Manager Class
	//========================================================================

	static std::unordered_map<std::type_index, std::vector<Event::HandlerBase*>> s_event_handlers;

	static bool comparator(Event::HandlerBase* a, Event::HandlerBase* b) { return a->m_priority > b->m_priority; }

	void Event::Manager::Subscribe(std::type_index type, HandlerBase* handler) {
		auto iter = s_event_handlers.find(type);
		if (iter != s_event_handlers.end()) {
			std::vector<HandlerBase*>& handlers = iter->second;
			handlers.insert(std::upper_bound(handlers.begin(), handlers.end(), handler, comparator), handler);
		}
		else {
			s_event_handlers[type] = { handler };
		}
	}

	void Event::Manager::Unsubscribe(std::type_index type, HandlerBase* handler) {
		auto iter = s_event_handlers.find(type);
		if (iter != s_event_handlers.end()) {
			std::vector<HandlerBase*>& handlers = iter->second;
			handlers.erase(std::find(handlers.begin(), handlers.end(), handler));
		}
	}

	void Event::Manager::Dispatch(std::type_index type, const EventBase& event) {
		auto iter = s_event_handlers.find(type);
		if (iter != s_event_handlers.end()) {
			for (HandlerBase* handler : iter->second) {
				if (handler->Execute(event)) {
					break;
				}
			}
		}
	}
}