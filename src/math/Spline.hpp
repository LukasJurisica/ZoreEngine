#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace zm {

	struct Curve {
		glm::vec4 exponents;
		float threshold;
	};

	class Spline {
	public:
		Spline(std::vector<glm::vec2> points);

	private:
		std::vector<Curve> curves;
	};
}