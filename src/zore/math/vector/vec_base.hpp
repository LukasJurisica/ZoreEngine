#pragma once

#include "zore/math/math.hpp"
#include "zore/platform.hpp"
#include <ostream>

namespace zm {

	template<typename T, int N>
		requires (std::is_trivially_copyable_v<T>)
	struct vec_base;
}