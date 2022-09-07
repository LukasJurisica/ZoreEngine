#include "generation/Biome.hpp"
#include "math/WhiteNoise.hpp"
#include "math/CellNoise.hpp"
#include "fnl/fastnoise.hpp"
#include "debug/Debug.hpp"

namespace zore {

	struct BiomeCache {
		int cellX, cellY;
		ubyte biome;
		bool oceanAdj;
	};

	fnl::FastNoiseLite terrain;
	fnl::FastNoiseLite ocean;
	fnl::FastNoiseLite biomeOffset;
	zm::CellNoise biome;
	zm::CellNoise region;

	static constexpr int ChunkSizeWithBorder = Chunk::CHUNK_WIDTH + 2;

	void BiomeManager::Init(int seed) {
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

		biome.SetFrequency(0.01f);
		biome.SetSeed(seed);
		region.SetFrequency(0.2f);
		region.SetSeed(seed);
	}

	void BiomeManager::GenerateBiomeMap(ubyte* biomeMap, Chunk* chunk) {

		std::vector<BiomeCache> temp;
		BiomeCache prev;
		glm::ivec3 pos = chunk->GetPosition();

		for (int x = 0; x < ChunkSizeWithBorder; x++) {
			for (int z = 0; z < ChunkSizeWithBorder; z++) {

				// Domain Warp
				float xo = x + pos.x - 1.f;
				float zo = z + pos.z - 1.f;
				biomeOffset.DomainWarp(xo, zo);

				// Biome Determination
				zm::CellData biomeResult;
				biome.GetNoise(xo, zo, biomeResult);
				xo = biomeResult.cell.x + biomeResult.offset.x;
				zo = biomeResult.cell.y + biomeResult.offset.y;

				// Region Determination
				//zm::CellData regionResult;
				//region.GetNoise(xo, zo, regionResult);
				//xo = regionResult.cell.x + regionResult.offset.x;
				//zo = regionResult.cell.y + regionResult.offset.y;

				float n = zm::NormalizeNoise(ocean.GetNoise(xo, zo));
				ubyte b = n > 0.5f ? 0 : zm::Floor(zm::WhiteNoise::Eval1(biomeResult.cell) * 32) + 1;
				biomeMap[x * ChunkSizeWithBorder + z] = b;
			}
		}
	}
}