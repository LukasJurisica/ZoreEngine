#pragma once

#include "zore/events/Event.hpp"

namespace zore {

	struct WindowResizedEvent : public Event::EventBase {
		WindowResizedEvent(int width, int height) : width(width), height(height) {
			aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
		}
		int width, height;
		float aspect_ratio;
	};

	struct WindowMovedEvent : public Event::EventBase {
		WindowMovedEvent(int xpos, int ypos) : xpos(xpos), ypos(ypos) {}
		int xpos, ypos;
	};

	struct WindowFocusedEvent : public Event::EventBase {
		WindowFocusedEvent(bool focused) : focused(focused) {}
		bool focused;
	};
}