#pragma once

#include "zore/events/Event.hpp"

namespace zore {

	struct MousePressedEvent : public Event::EventBase {
		MousePressedEvent(int button, int mods) : button(button), mods(mods) {}
		int button, mods;
	};

	struct MouseReleasedEvent : public Event::EventBase {
		MouseReleasedEvent(int button) : button(button) {}
		int button;
	};

	struct MouseScrolledEvent : public Event::EventBase {
		MouseScrolledEvent(float x, float y) : dx(x), dy(y) {}
		float dx, dy;
	};

	struct MouseMovedEvent : public Event::EventBase {
		MouseMovedEvent(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy) {}
		float x, y, dx, dy;
	};

	struct MouseEnteredEvent : public Event::EventBase {
		MouseEnteredEvent(bool entered) : entered(entered) {}
		bool entered;
	};
}