#pragma once

#include "zore/events/Event.hpp"

namespace zore {

	struct MousePressedEvent : public event::EventBase {
		MousePressedEvent(int button, int mods) : button(button), mods(mods) {}
		int button, mods;
	};

	struct MouseReleasedEvent : public event::EventBase {
		MouseReleasedEvent(int button) : button(button) {}
		int button;
	};

	struct MouseScrolledEvent : public event::EventBase {
		MouseScrolledEvent(float x, float y) : dx(x), dy(y) {}
		float dx, dy;
	};

	struct MouseMovedEvent : public event::EventBase {
		MouseMovedEvent(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy) {}
		float x, y, dx, dy;
	};

	struct MouseEnteredEvent : public event::EventBase {
		MouseEnteredEvent(bool entered) : entered(entered) {}
		bool entered;
	};
}