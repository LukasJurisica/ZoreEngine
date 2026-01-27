#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/concepts.hpp"
#include "zore/platform.hpp"
#include <cmath>
#include <ostream>

namespace zm {

	template<typename T, int N>
		requires (std::is_trivially_copyable_v<T>)
	struct vec_base;
}