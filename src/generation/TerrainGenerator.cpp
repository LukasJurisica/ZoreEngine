#include "generation/TerrainGenerator.hpp"
#include "generation/Biome.hpp"
#include "math/WhiteNoise.hpp"
#include "math/CellNoise.hpp"
#include "fnl/fastnoise.hpp"
#include "debug/Debug.hpp"

namespace zore {

	static constexpr int OCEAN_LEVEL = 40;
	static constexpr float OCEAN_THRESH = 0.5f;
	static constexpr int CHUNK_WIDTH_WITH_BORDER = CHUNK_WIDTH + 2;
	static constexpr int CHUNK_HSLICE_WITH_BORDER = CHUNK_WIDTH_WITH_BORDER * CHUNK_WIDTH_WITH_BORDER;

	static int worldSeed = 12345;
	static fnl::FastNoiseLite terrain;
	static fnl::FastNoiseLite ocean;
	static fnl::FastNoiseLite biomeOffset;
	static zm::CellNoise biomeNoise;
	static zm::CellNoise regionNoise;

	//========================================================================
	// Terrain Generator Class
	//========================================================================

	void TerrainGenerator::Init(int seed) {
		worldSeed = seed;
		BiomeManager::Init();

		terrain.SetSeed(seed);
		terrain.SetNoiseType(fnl::NoiseType::OpenSimplex2S);
		terrain.SetFrequency(0.003f);
		terrain.SetFractalOctaves(3);
		terrain.SetFractalType(fnl::FractalType::Ridged);

		ocean.SetSeed(seed);
		ocean.SetNoiseType(fnl::NoiseType::Value);
		ocean.SetFrequency(0.2f);
		ocean.SetFractalOctaves(2);
		ocean.SetFractalType(fnl::FractalType::FBm);

		biomeOffset.SetSeed(seed);
		biomeOffset.SetDomainWarpType(fnl::DomainWarpType::BasicGrid);
		biomeOffset.SetDomainWarpAmp(50);
		biomeOffset.SetFractalType(fnl::FractalType::DomainWarpProgressive);
		biomeOffset.SetFractalOctaves(5);

		biomeNoise.SetFrequency(0.01f);
		biomeNoise.SetSeed(seed);
		regionNoise.SetFrequency(0.2f);
		regionNoise.SetSeed(seed);
	}

	void TerrainGenerator::Generate(Chunk* chunk) {
		if (!chunk->blockData)
			chunk->blockData = new ushort[CHUNK_VOLUME];

		ubyte biomeMap[CHUNK_HSLICE_WITH_BORDER];
		ubyte heightMap[CHUNK_HSLICE_WITH_BORDER];
		glm::ivec3 offset = chunk->renderPos;
		GenerateBiomeMap(biomeMap, offset);
		GenerateHeightMap(heightMap, biomeMap, offset);

		for (int x = 0; x < CHUNK_WIDTH; x++) {
			for (int z = 0; z < CHUNK_WIDTH; z++) {
				int index = (x + 1) * CHUNK_WIDTH_WITH_BORDER + z + 1;
				int h = heightMap[index];
				Biome* biome = BiomeManager::GetBiome(biomeMap[index]);

				// Determine Surface Block
				ushort surface = biome->GetSurfaceBlock();

				// Build up core
				for (int y = 0; y <= h - 4; y++)
					chunk->SetBlockLocal(x, y, z, BLOCK_STONE);
				for (int y = h - 3; y <= h - 1; y++)
					chunk->SetBlockLocal(x, y, z, BLOCK_DIRT);
				chunk->SetBlockLocal(x, h, z, surface);

				// Place water and air above world
				for (int y = h + 1; y <= OCEAN_LEVEL; y++)
					chunk->SetBlockLocal(x, y, z, FLUID_WATER);
				for (int y = zm::Max(OCEAN_LEVEL, h) + 1; y < CHUNK_HEIGHT; y++)
					chunk->SetBlockLocal(x, y, z, BLOCK_AIR);

				// Place plants
				if (surface == BLOCK_GRASS && h >= OCEAN_LEVEL) {
					float p = zm::WhiteNoise::Eval1(glm::vec2(x + chunk->renderPos.x, z + chunk->renderPos.y));
					if (p > 0.995)
						chunk->SetBlockLocal(x, h + 1, z, SPRITE_MUSHROOM);
					else if (p > 0.85)
						chunk->SetBlockLocal(x, h + 1, z, SPRITE_PLANT);
				}
			}
		}
	}

	void TerrainGenerator::GenerateBiomeMap(ubyte* biomeMap, glm::ivec3& offset) {
		for (int x = 0; x < CHUNK_WIDTH_WITH_BORDER; x++)
			for (int z = 0; z < CHUNK_WIDTH_WITH_BORDER; z++)
				biomeMap[x * CHUNK_WIDTH_WITH_BORDER + z] = GetBiome(x + offset.x - 1, z + offset.z - 1);
	}

	void TerrainGenerator::GenerateHeightMap(ubyte* heightMap, ubyte* biomeMap, glm::ivec3& offset) {
		for (int x = 0; x < CHUNK_WIDTH_WITH_BORDER; x++) {
			for (int z = 0; z < CHUNK_WIDTH_WITH_BORDER; z++) {
				Biome* biome = BiomeManager::GetBiome(biomeMap[x * CHUNK_WIDTH_WITH_BORDER + z]);
				heightMap[x * CHUNK_WIDTH_WITH_BORDER + z] = biome->GetHeight(x + offset.x - 1, z + offset.z - 1);
			}
		}
	}

	int TerrainGenerator::GetSeed() {
		return worldSeed;
	}

	ubyte TerrainGenerator::GetOceanAltitude() {
		return OCEAN_LEVEL;
	}

	ubyte TerrainGenerator::GetBiome(int x, int z) {
		float xo = static_cast<float>(x), zo = static_cast<float>(z);
		biomeOffset.DomainWarp(xo, zo);

		// Biome Cell Determination
		zm::CellData biomeResult;
		biomeNoise.GetNoise(xo, zo, biomeResult);
		xo = biomeResult.cell.x + biomeResult.offset.x;
		zo = biomeResult.cell.y + biomeResult.offset.y;

		// Region Cell Determination
		//zm::CellData regionResult;
		//regionNoise.GetNoise(xo, zo, regionResult);
		//xo = regionResult.cell.x + regionResult.offset.x;
		//zo = regionResult.cell.y + regionResult.offset.y;

		// Ocean Determination
		float n = zm::NormalizeNoise(ocean.GetNoise(xo, zo));
		BiomeType bt = (n > OCEAN_THRESH) ? BiomeType::OCEAN : BiomeType::LAND;

		// Biome ID Determination
		float rand = zm::WhiteNoise::GetNoise(biomeResult.cell.x, biomeResult.cell.y, worldSeed);
		return BiomeManager::GetBiomeID(rand, bt);
	}
}