#pragma once
#include "voxel/Chunk.hpp"

namespace zore {

	//========================================================================
	// Terrain Generator Class
	//========================================================================
	
	struct BiomeCache {
		int biomeCellX, biomeCellY;
		ubyte biome;
		int regionCellX, regionCellY;
		ubyte region;
		bool oceanAdj;
	};

	class TerrainGenerator {
	public:
		static void Init(int seed);

		static void Generate(Chunk* chunk);
		static void GenerateBiomeMap(ubyte* biomeMap, glm::ivec3& offset);
		static void GenerateHeightMap(ubyte* heightMap, ubyte* biomeMap, glm::ivec3& offset);
		
		static int GetSeed();
		static ubyte GetOceanAltitude();
		static ubyte GetBiome(int x, int z);
	};
}