#pragma once

namespace zore {

	template <typename TYPE, int DIMS>
	class Point {
	public:

	private:
		TYPE[DIMS] data;
	};

	template <typename TYPE, int DIMS>
	class AABB {
	public:

	private:
		Point<TYPE, DIMS> min;
		Point<TYPE, DIMS> max;
	};
}