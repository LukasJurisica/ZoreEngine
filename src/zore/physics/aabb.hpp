#pragma once

#include "zore/math/vector.hpp"

namespace zore {

	template <typename TYPE, int DIMS>
	class AABB {
	public:

	private:
		zm::vec_base <TYPE, DIMS> min;
		zm::vec_base <TYPE, DIMS> max;
	};
}