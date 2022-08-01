#include "voxel/TerrainGenerator.hpp"
#include "math/WhiteNoise.hpp"
#include "math/CellNoise.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

#include <platform/win32/win32_Core.hpp>

namespace zore {

	static constexpr int ChunkSizeWithBorder = Chunk::CHUNK_WIDTH + 2;

#define SEED 123

	//========================================================================
	// Terrain Generator Class
	//========================================================================

	TerrainGenerator::TerrainGenerator() : terrain(SEED), ocean(SEED), biomeOffset(SEED), biome(0.01f, 0.f, SEED) {
		terrain.SetNoiseType(fnl::NoiseType::OpenSimplex2S);
		terrain.SetFrequency(0.003f);
		terrain.SetFractalOctaves(3);
		terrain.SetFractalType(fnl::FractalType::Ridged);
		
		ocean.SetNoiseType(fnl::NoiseType::Value);
		ocean.SetFrequency(0.2f);
		ocean.SetFractalOctaves(2);
		ocean.SetFractalType(fnl::FractalType::FBm);

		biomeOffset.SetDomainWarpType(fnl::DomainWarpType::BasicGrid);
		biomeOffset.SetDomainWarpAmp(50);
		biomeOffset.SetFractalType(fnl::FractalType::DomainWarpProgressive);
		biomeOffset.SetFractalOctaves(5);
	}

	void TerrainGenerator::Generate(Chunk* chunk) {
		if (!chunk->blockData)
			chunk->blockData = new ushort[Chunk::CHUNK_VOLUME];

		ubyte biomeMap[ChunkSizeWithBorder * ChunkSizeWithBorder];
		ushort heightMap[ChunkSizeWithBorder * ChunkSizeWithBorder];
		GenerateBiomeMap(biomeMap, chunk);
		GenerateHeightMap(heightMap, biomeMap, chunk);

		for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
			for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {

				int index = (x + 1) * ChunkSizeWithBorder + z + 1;
				int h = heightMap[index];

				// Determine Surface Block
				//ushort surface = biomeMap[index] == 0 ? BLOCK_GRASS : BLOCK_CLAY;
				ushort surface = BLOCK_GRASS;
				if (zm::Abs(heightMap[index - 1] - heightMap[index + 1]) > 1 || zm::Abs(heightMap[index - ChunkSizeWithBorder] - heightMap[index + ChunkSizeWithBorder]) > 1)
					surface = BLOCK_DIRT;

				// Build up core
				for (int y = 0; y <= h - 4; y++)
					chunk->SetBlockLocal(x, y, z, BLOCK_STONE);
				for (int y = h - 3; y <= h - 1; y++)
					chunk->SetBlockLocal(x, y, z, BLOCK_DIRT);
				chunk->SetBlockLocal(x, h, z, surface);

				// Place water and air above world
				int waterHeight = 40;
				for (int y = h + 1; y <= waterHeight; y++)
					chunk->SetBlockLocal(x, y, z, FLUID_WATER);
				for (int y = zm::Max(waterHeight, h) + 1; y < Chunk::CHUNK_HEIGHT; y++)
					chunk->SetBlockLocal(x, y, z, BLOCK_AIR);

				// Place plants
				if (surface == BLOCK_GRASS && h > waterHeight) {
					float p = zm::WhiteNoise::Eval1(glm::vec2(x, z));
					if (p > 0.99)
						chunk->SetBlockLocal(x, h + 1, z, SPRITE_MUSHROOM);
					else if (p > 0.85)
						chunk->SetBlockLocal(x, h + 1, z, SPRITE_PLANT);
				}
			}
		}
	}

	void TerrainGenerator::GenerateBiomeMap(ubyte* biomeMap, Chunk* chunk) {

		std::vector<SubBiomeCache> temp;
		SubBiomeCache prev;

		for (int x = 0; x < ChunkSizeWithBorder; x++) {
			for (int z = 0; z < ChunkSizeWithBorder; z++) {
				
				// Domain Warp
				float xo = x + chunk->renderPos.x - 1;
				float zo = z + chunk->renderPos.z - 1;
				biomeOffset.DomainWarp(xo, zo);
				
				// Sub Biome Determination
				zm::CellData subBiomeResult;
				biome.GetNoise(xo, zo, subBiomeResult);

				// Parent Determination
				SubBiomeCache subBiomeCache;
				for (const SubBiomeCache& b : temp) {

				}

				xo = subBiomeResult.cell.x + subBiomeResult.offset.x;
				zo = subBiomeResult.cell.y + subBiomeResult.offset.y;
				float n = zm::NormalizeNoise(ocean.GetNoise(xo, zo));

				ubyte b = n > 0.5f ? 0 : zm::Floor(zm::WhiteNoise::Eval1(subBiomeResult.cell) * 32) + 1;
				biomeMap[x * ChunkSizeWithBorder + z] = b;
			}
		}
	}

	void TerrainGenerator::GenerateHeightMap(ushort* heightMap, ubyte* biomeMap, Chunk* chunk) {
		for (int x = 0; x < ChunkSizeWithBorder; x++) {
			for (int z = 0; z < ChunkSizeWithBorder; z++) {

				//float n = ocean.GetNoise(x + chunk->renderPos.x - 1.f, z + chunk->renderPos.z - 1.f);
				float n = terrain.GetNoise(x + chunk->renderPos.x - 1.f, z + chunk->renderPos.z - 1.f);
				//ushort h = zm::Floor(zm::SmoothMax(0.8f, zm::NormalizeNoise(-n) * 3.2f, 0.5f) * 20);
				//ushort h = zm::Floor(zm::Max(16.f, zm::NormalizeNoise(-n) * 64));
				//ushort h = zm::Floor(zm::SmoothMax(24.f, zm::NormalizeNoise(n) * 128.f, 15.f));

				ubyte b = biomeMap[x * ChunkSizeWithBorder + z];
				ushort h = b == 0 ? 20 : b * 2 + 40;
				heightMap[x * ChunkSizeWithBorder + z] = h;
			}
		}
	}
}