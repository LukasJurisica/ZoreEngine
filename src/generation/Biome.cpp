#include "generation/Biome.hpp"
#include "generation/TerrainGenerator.hpp"
#include "fnl/fastnoise.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

namespace zore {

	static fnl::FastNoiseLite terrain;
	static std::vector<Biome*> biomes;
	static int oceanBiomeCount = 0;
	static float oceanHeight = 0;

	//========================================================================
	//	Biome Manager Class
	//========================================================================

	void BiomeManager::Init() {
		Register(new Ocean(), BiomeType::OCEAN);
		Register(new Desert(), BiomeType::LAND);
		Register(new Forest(), BiomeType::LAND);

		oceanHeight = TerrainGenerator::GetOceanAltitude();

		terrain.SetSeed(TerrainGenerator::GetSeed());
		terrain.SetNoiseType(fnl::NoiseType::OpenSimplex2S);
		terrain.SetFrequency(0.003f);
		terrain.SetFractalOctaves(3);
		terrain.SetFractalType(fnl::FractalType::Ridged);
	}

	void BiomeManager::Register(Biome* biome, BiomeType type) {
		if (type == BiomeType::OCEAN)
			biomes.insert(biomes.begin() + (oceanBiomeCount++), biome);
		else
			biomes.emplace_back(biome);
	}

	biome_t BiomeManager::GetBiomeID(float rand, BiomeType type) {
		rand = zm::Fract(rand);
		if (type == BiomeType::OCEAN)
			return zm::Floor(rand * oceanBiomeCount);
		else
			return zm::Floor(rand * (biomes.size() - oceanBiomeCount)) + oceanBiomeCount;
	}

	Biome* BiomeManager::GetBiome(biome_t id) {
		return biomes[id];
	}

	//========================================================================
	//	Custom Biome Classes
	//========================================================================

	Ocean::Ocean() {}

	ubyte Ocean::GetHeight(int x, int z) const {
		return 20;
	}

	block_t Ocean::GetSurfaceBlock() const {
		return BLOCK_SAND;
	}


	Desert::Desert() {}

	ubyte Desert::GetHeight(int x, int z) const {
		float n = terrain.GetNoise(static_cast<float>(x), static_cast<float>(z));
		return zm::Floor(zm::SmoothMax(oceanHeight, zm::NormalizeNoise(n) * 128.f, 15.f));
	}

	block_t Desert::GetSurfaceBlock() const {
		return BLOCK_SAND;
	}


	Forest::Forest() {}

	ubyte Forest::GetHeight(int x, int z) const {
		float n = terrain.GetNoise(static_cast<float>(x), static_cast<float>(z));
		return zm::Floor(zm::SmoothMax(oceanHeight, zm::NormalizeNoise(n) * 128.f, 15.f));
	}

	block_t Forest::GetSurfaceBlock() const {
		return BLOCK_GRASS;
	}
}