#pragma once
#include "voxel/Block.hpp"
#include <vector>



namespace zore {

	class Blueprint {
	public:
		Blueprint();
		~Blueprint() = default;

	private:
		struct blockInstance {
			ushort blockID;
			ubyte x, y, z;
		};

		std::vector<blockInstance> sparseData;
		std::vector<ushort> denseData;
	};
}