#include "zore/math/Spline.hpp"
#include "zore/math/matrix/mat4.hpp"
#include "zore/Debug.hpp"

namespace zm {

	Spline::Spline(std::vector<zm::vec2>& points) {
		zm::mat4 m;
		zm::vec4 y;
		for (int i = 0; i < 4; i++) {
			float x = points[i].x;
			y[i] = points[i].y;
			m[0][i] = 1.f;
			m[1][i] = x;
			m[2][i] = x * x;
			m[3][i] = x * x * x;
		}

		y = zm::Inverse(m) * y;
	}
}