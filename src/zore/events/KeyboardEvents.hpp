#pragma once

#include "zore/events/Event.hpp"

namespace zore {

	struct KeyPressedEvent : public Event::EventBase {
		KeyPressedEvent(int key, int mods) : key(key), mods(mods) {}
		int key, mods;
	};

	struct KeyReleasedEvent : public Event::EventBase {
		KeyReleasedEvent(int key) : key(key) {}
		int key;
	};
}