#pragma once
#include "fnl/fastnoise.hpp"
//#include "fnl/fastnoiselite.h"
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
		void GenerateHeightmap(short* heightMap, Chunk* chunk);

	private:
		fnl::FastNoiseLite terrain;
		fnl::FastNoiseLite biome;
	};
}