#include "voxel/Chunk.hpp"
typedef ubyte biome_t;

namespace zore {

	//========================================================================
	//	Base Biome Class
	//========================================================================

	enum class BiomeType { OCEAN, LAND };

	class Biome {
	public:
		~Biome() = default;

		virtual ubyte GetHeight(int x, int z) const = 0;
		virtual block_t GetSurfaceBlock() const = 0;
	};

	//========================================================================
	//	Biome Manager Class
	//========================================================================

	class BiomeManager {
	public:
		static void Init();
		static void Register(Biome* biome, BiomeType type);
		static biome_t GetBiomeID(float rand, BiomeType type);
		static Biome* GetBiome(biome_t id);
	};

	//========================================================================
	//	Custom Biome Classes
	//========================================================================

	class Ocean : public Biome {
	public:
		Ocean();
		~Ocean() = default;

		ubyte GetHeight(int x, int z) const override;
		block_t GetSurfaceBlock() const override;
	};

	class Desert : public Biome {
	public:
		Desert();
		~Desert() = default;

		ubyte GetHeight(int x, int z) const override;
		block_t GetSurfaceBlock() const override;
	};

	class Forest : public Biome {
	public:
		Forest();
		~Forest() = default;

		ubyte GetHeight(int x, int z) const override;
		block_t GetSurfaceBlock() const override;
	};
}