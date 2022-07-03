#pragma once
#include "fnl/fastnoise.hpp"
#include "math/CellNoise.hpp"
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
		void GenerateBHMap(ubyte* biomeMap, ushort* heightMap, Chunk* chunk);

	private:
		fnl::FastNoiseLite terrain;
		//fnl::FastNoiseLite biome;
		zm::CellNoise biome;
	};
}