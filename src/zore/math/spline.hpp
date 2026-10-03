#pragma once
#include <vector>
#include "zore/math/vector/vec2.hpp"
#include "zore/math/vector/vec4.hpp"

namespace zm {

	struct Curve {
		zm::vec4 exponents;
		float threshold;
	};

	class Spline {
	public:
		Spline(std::vector<zm::vec2>& points);

	private:
		std::vector<Curve> curves;
	};
}