#pragma once
#include "math/CoherentNoise.hpp"
#include "voxel/Chunk.hpp"

namespace zore {

	//========================================================================
	// Terrain Generator Class
	//========================================================================

	class TerrainGenerator {
	public:
		TerrainGenerator();
		~TerrainGenerator() = default;

		void Generate(Chunk* chunk);

	private:
		zm::SimplexGenerator sg;
		zm::PerlinGenerator pg;
	};
}