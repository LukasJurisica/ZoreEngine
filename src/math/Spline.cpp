#include "math/Spline.hpp"
#include "debug/Debug.hpp"

#include <glm/mat4x4.hpp>

namespace zm {

	Spline::Spline(std::vector<glm::vec2> points) {
		glm::mat4 m;
		glm::vec4 y;
		for (int i = 0; i < 4; i++) {
			float x = points[i].x;
			y[i] = points[i].y;
			m[0][i] = 1.f;
			m[1][i] = x;
			m[2][i] = x * x;
			m[3][i] = x * x * x;
		}

		y = glm::inverse(m) * y;
	}
}