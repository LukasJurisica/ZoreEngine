#pragma once
#include <glm/vec2.hpp>
#include <vector>

namespace zm {

	class Bezier {
	public:
		// Returns a set of {count} points that pass through end points a and b, augmented by control point c
		static void Quadratic(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, int count, glm::vec2* out, bool includeEndPoints = true);

		// Returns a set of {count} points that pass through end points a and b, augmented by control points c and d
		static void Cubic(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, int count, glm::vec2* out, bool includeEndPoints = true);
	};
}