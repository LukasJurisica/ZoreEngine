#include "generation/TerrainGenerator.hpp"
#include "generation/Biome.hpp"
#include "math/WhiteNoise.hpp"
#include "fnl/fastnoise.hpp"
#include "debug/Debug.hpp"

namespace zore {

	static constexpr int ChunkSizeWithBorder = Chunk::CHUNK_WIDTH + 2;
	static constexpr int OceanLevel = 40;
	static fnl::FastNoiseLite terrain;


#define SEED 123

	//========================================================================
	// Terrain Generator Class
	//========================================================================

	void TerrainGenerator::Init(int seed) {
		BiomeManager::Init(seed);

		terrain.SetSeed(seed);
		terrain.SetNoiseType(fnl::NoiseType::OpenSimplex2S);
		terrain.SetFrequency(0.003f);
		terrain.SetFractalOctaves(3);
		terrain.SetFractalType(fnl::FractalType::Ridged);
	}

	void TerrainGenerator::Generate(Chunk* chunk) {
		if (!chunk->blockData)
			chunk->blockData = new ushort[Chunk::CHUNK_VOLUME];

		ubyte biomeMap[ChunkSizeWithBorder * ChunkSizeWithBorder];
		ushort heightMap[ChunkSizeWithBorder * ChunkSizeWithBorder];
		BiomeManager::GenerateBiomeMap(biomeMap, chunk);
		GenerateHeightMap(heightMap, biomeMap, chunk);

		for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
			for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {

				int index = (x + 1) * ChunkSizeWithBorder + z + 1;
				int h = heightMap[index];

				// Determine Surface Block
				ushort surface = BLOCK_GRASS;
				if (zm::Abs(heightMap[index - 1] - heightMap[index + 1]) > 2 || zm::Abs(heightMap[index - ChunkSizeWithBorder] - heightMap[index + ChunkSizeWithBorder]) > 2)
					surface = BLOCK_DIRT;

				// Build up core
				for (int y = 0; y <= h - 4; y++)
					chunk->SetBlockLocal(x, y, z, BLOCK_STONE);
				for (int y = h - 3; y <= h - 1; y++)
					chunk->SetBlockLocal(x, y, z, BLOCK_DIRT);
				chunk->SetBlockLocal(x, h, z, surface);

				// Place water and air above world
				for (int y = h + 1; y <= OceanLevel; y++)
					chunk->SetBlockLocal(x, y, z, FLUID_WATER);
				for (int y = zm::Max(OceanLevel, h) + 1; y < Chunk::CHUNK_HEIGHT; y++)
					chunk->SetBlockLocal(x, y, z, BLOCK_AIR);

				// Place plants
				if (surface == BLOCK_GRASS && h >= OceanLevel) {
					float p = zm::WhiteNoise::Eval1(glm::vec2(x + chunk->renderPos.x, z + chunk->renderPos.y));
					if (p > 0.99)
						chunk->SetBlockLocal(x, h + 1, z, SPRITE_MUSHROOM);
					else if (p > 0.85)
						chunk->SetBlockLocal(x, h + 1, z, SPRITE_PLANT);
				}
			}
		}
	}

	void TerrainGenerator::GenerateHeightMap(ushort* heightMap, ubyte* biomeMap, Chunk* chunk) {
		for (int x = 0; x < ChunkSizeWithBorder; x++) {
			for (int z = 0; z < ChunkSizeWithBorder; z++) {

				float n = terrain.GetNoise(x + chunk->renderPos.x - 1.f, z + chunk->renderPos.z - 1.f);
				ubyte b = biomeMap[x * ChunkSizeWithBorder + z];

				ushort h = zm::Floor(zm::SmoothMax(static_cast<float>(OceanLevel), zm::NormalizeNoise(n) * 128.f, 15.f));
				//ushort h = b == 0 ? 20 : b * 2 + 40;

				if (b == 0)
					h = 20;

				heightMap[x * ChunkSizeWithBorder + z] = h;
			}
		}
	}
}