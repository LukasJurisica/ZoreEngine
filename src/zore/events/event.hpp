#pragma once

namespace zore::event {

	//========================================================================
	//	Event Base Class
	//========================================================================

	struct EventBase {
		virtual ~EventBase() = default;
	};
}