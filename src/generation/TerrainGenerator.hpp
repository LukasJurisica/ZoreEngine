#pragma once
#include "voxel/Chunk.hpp"

namespace zore {

	struct BiomeCache {
		int cellX, cellY;
		ubyte biome;
		bool oceanAdj;
	};

	//========================================================================
	// Terrain Generator Class
	//========================================================================

	class TerrainGenerator {
	public:
		static void Init(int seed);

		static void Generate(Chunk* chunk);
		static void GenerateHeightMap(ushort* heightMap, ubyte* biomeMap, Chunk* chunk);
	};
}