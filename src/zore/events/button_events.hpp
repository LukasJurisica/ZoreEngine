#pragma once

#include "zore/events/event.hpp"

namespace zore {

	struct ButtonPressedEvent : public event::Base {
		ButtonPressedEvent(uint32_t uuid) : uuid(uuid) {}
		uint32_t uuid;
	};

	struct ButtonReleasedEvent : public event::Base {
		ButtonReleasedEvent(uint32_t uuid) : uuid(uuid) {}
		uint32_t uuid;
	};
}