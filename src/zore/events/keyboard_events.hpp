#pragma once

#include "zore/events/event.hpp"

namespace zore {

	struct KeyPressedEvent : public event::Base {
		KeyPressedEvent(int key, int mods) : key(key), mods(mods) {}
		int key, mods;
	};

	struct KeyReleasedEvent : public event::Base {
		KeyReleasedEvent(int key) : key(key) {}
		int key;
	};
}