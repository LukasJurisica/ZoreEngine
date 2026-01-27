#pragma once
#include "zore/math/vector/vec2.hpp"

namespace zm {

	class Bezier {
	public:
		// Returns a set of {count} points that pass through end points a and b, augmented by control point c
		static void Quadratic(const zm::vec2& a, const zm::vec2& b, const zm::vec2& c, int count, zm::vec2* out, bool includeEndPoints = true);

		// Returns a set of {count} points that pass through end points a and b, augmented by control points c and d
		static void Cubic(const zm::vec2& a, const zm::vec2& b, const zm::vec2& c, const zm::vec2& d, int count, zm::vec2* out, bool includeEndPoints = true);
	};
}