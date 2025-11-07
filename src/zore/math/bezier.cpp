#include "zore/math/bezier.hpp"
#include "zore/debug.hpp"

namespace zm {

	void Bezier::Quadratic(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, int count, glm::vec2* out, bool includeEndPoints) {

	}

	void Bezier::Cubic(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, int count, glm::vec2* out, bool includeEndPoints) {

	}

	//void Bezier::Quadratic(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, int count, glm::vec2* out, bool includeEndPoints) {
	//	DEBUG_ENSURE(count >= 2, "Point count on bezier curve must be atleast 2");
	//	int steps = count - 1;
	//	float t = 0.f;
	//	float s = 1.f / steps;
	//	int offset = 0;

	//	if (includeEndPoints) {
	//		out[0] = a;
	//		out[steps] = c;
	//		offset = 1;
	//	}

	//	for (int i = 0; i < steps - 1; i++) {
	//		out[i + offset] = a + (2.f * t * (b - a)) + (t * t * (c - (2.f * b) + a));
	//		t += s;
	//	}
	//}

	//void Bezier::Cubic(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, int count, glm::vec2* out, bool includeEndPoints) {
	//	DEBUG_ENSURE(count >= 2, "Point count on bezier curve must be atleast 2");
	//	int steps = count - 1;
	//	float t = 0.f;
	//	float s = 1.f / steps;
	//	int offset = 0;

	//	if (includeEndPoints) {
	//		out[0] = a;
	//		out[steps] = c;
	//		offset = 1;
	//	}

	//	for (int i = 0; i < steps - 1; i++) {
	//		float ts = t * t;
	//		out[i + offset] = a + (3.f * t * (b - a)) + (3.f * ts * (c - (2.f * b) + a)) + (ts * t * (d - (3.f * c) + (3.f * b) - a));
	//		t += s;
	//	}
	//}
}