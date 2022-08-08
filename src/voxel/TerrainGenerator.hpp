#pragma once
#include "fnl/fastnoise.hpp"
#include "math/CellNoise.hpp"
#include "voxel/Chunk.hpp"

namespace zore {

	//========================================================================
	// Terrain Generator Class
	//========================================================================

	struct SubBiomeCache {
		int cellX, cellY;
		ubyte biome;
		bool oceanAdj;
	};

	class TerrainGenerator {
	public:
		TerrainGenerator();
		~TerrainGenerator() = default;

		void Generate(Chunk* chunk);
		void GenerateBiomeMap(ubyte* biomeMap, Chunk* chunk);
		void GenerateHeightMap(ushort* heightMap, ubyte* biomeMap, Chunk* chunk);

	private:
		fnl::FastNoiseLite terrain;
		fnl::FastNoiseLite ocean;
		fnl::FastNoiseLite biomeOffset;
		zm::CellNoise biome;
		zm::CellNoise subBiome;
	};
}