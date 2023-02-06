#pragma once
#include <glm/vec2.hpp>

namespace zm {

	class Bezier {
	public:
		static void Quadratic(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, int count, glm::vec2* out, bool includeEndPoints = true);
		static void Cubic(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, int count, glm::vec2* out, bool includeEndPoints = true);
	};
}