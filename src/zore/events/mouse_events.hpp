#pragma once

#include "zore/events/event.hpp"

namespace zore {

	struct MousePressedEvent : public event::Base {
		MousePressedEvent(int button, int mods) : button(button), mods(mods) {}
		int button, mods;
	};

	struct MouseReleasedEvent : public event::Base {
		MouseReleasedEvent(int button) : button(button) {}
		int button;
	};

	struct MouseScrolledEvent : public event::Base {
		MouseScrolledEvent(float x, float y) : dx(x), dy(y) {}
		float dx, dy;
	};

	struct MouseMovedEvent : public event::Base {
		MouseMovedEvent(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy) {}
		float x, y, dx, dy;
	};

	struct MouseEnteredEvent : public event::Base {
		MouseEnteredEvent(bool entered) : entered(entered) {}
		bool entered;
	};
}