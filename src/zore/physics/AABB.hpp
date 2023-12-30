#pragma once

namespace zore {

	template <int DIMS>
	class Point {
	public:

	private:
		float[DIMS] data;
	};

	template <int DIMS>
	class AABB {
	public:

	private:
		Point<DIMS> min;
		Point<DIMS> max;
	};
}