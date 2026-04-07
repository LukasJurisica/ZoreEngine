#pragma once

#include <concepts>

namespace zore::event {

	//========================================================================
	//	Event Base
	//========================================================================

	struct Base {
		virtual ~Base() = default;
	};

	template <typename T>
	concept Derivative = std::derived_from<T, event::Base>;
}