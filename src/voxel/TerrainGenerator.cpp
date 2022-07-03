#include "voxel/TerrainGenerator.hpp"
#include "math/WhiteNoise.hpp"
#include "math/CellNoise.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

#include <platform/win32/win32_Core.hpp>

namespace zore {

	static constexpr int ChunkSizeWithBorder = Chunk::CHUNK_WIDTH + 2;

	//========================================================================
	// Terrain Generator Class
	//========================================================================

	TerrainGenerator::TerrainGenerator() : terrain(123), biome(0.005f, 321) {
		terrain.SetNoiseType(fnl::NoiseType::OpenSimplex2S);
		terrain.SetFrequency(0.003f);
		terrain.SetFractalType(fnl::FractalType::Ridged);
		//terrain.SetFractalLacunarity(2.f);
		terrain.SetFractalOctaves(3);

		biome.SetCentralBias(0.3f);

		//biome.SetNoiseType(fnl::NoiseType::Cellular);
		//biome.SetCellularDistanceFunction(fnl::CellularDistanceFunction::Hybrid);
		//biome.SetCellularReturnType(fnl::CellularReturnType::CellValue);
		//biome.SetCellularJitter(1.f);
		//biome.SetDomainWarpType(fnl::DomainWarpType::OpenSimplex2);
		//biome.SetDomainWarpAmp(100);
		//biome.SetFractalType(fnl::FractalType::DomainWarpIndependent);
	}

	void TerrainGenerator::Generate(Chunk* chunk) {
		if (!chunk->blockData)
			chunk->blockData = new ushort[Chunk::CHUNK_VOLUME];

		ubyte biomeMap[ChunkSizeWithBorder * ChunkSizeWithBorder];
		ushort heightMap[ChunkSizeWithBorder * ChunkSizeWithBorder];
		GenerateBHMap(biomeMap, heightMap, chunk);

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

	void TerrainGenerator::GenerateBHMap(ubyte* biomeMap, ushort* heightMap, Chunk* chunk) {
		for (int x = 0; x < ChunkSizeWithBorder; x++) {
			for (int z = 0; z < ChunkSizeWithBorder; z++) {

				//int h = zm::Floor(zm::SmoothMax(0.8f, zm::NormalizeNoise(-n) * 3.2f, 0.5f) * 20);
				//int h = zm::Floor(zm::Max(16.f, zm::NormalizeNoise(-n) * 64));

				//float n = terrain.GetNoise(x + chunk->renderPos.x - 1.f, z + chunk->renderPos.z - 1.f);
				//int h = zm::Floor(zm::SmoothMax(24.f, zm::NormalizeNoise(n) * 128.f, 15.f));
				//int b = zm::Floor(zm::NormalizeNoise(biome.GetNoise(x + chunk->renderPos.x, z + chunk->renderPos.z)) * 20) * 2;

				zm::CellData res;
				biome.GetNoise(x + chunk->renderPos.x - 1, z + chunk->renderPos.z - 1, res);
				biomeMap[x * ChunkSizeWithBorder + z] = zm::Floor(zm::WhiteNoise::Eval1(res.cell) * 16);

				ushort h = biomeMap[x * ChunkSizeWithBorder + z] * 3 + 40;
				heightMap[x * ChunkSizeWithBorder + z] = h;
			}
		}
	}
}