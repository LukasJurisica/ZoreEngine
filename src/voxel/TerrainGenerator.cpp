#include "voxel/TerrainGenerator.hpp"
#include "math/WhiteNoise.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

#include <platform/win32/win32_Core.hpp>

zm::SimplexGenerator dw(123);

namespace zore {

	//========================================================================
	// Terrain Generator Class
	//========================================================================

	TerrainGenerator::TerrainGenerator() : sg(123) {
		sg.SetFrequency(0.005f);
		sg.SetOctaves(3);

		dw.SetFrequency(0.001f);
	}

	void TerrainGenerator::Generate(Chunk* chunk) {
		if (!chunk->blockData)
			chunk->blockData = new ushort[Chunk::CHUNK_VOLUME];

		for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
			for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {

				//float a = (dw.Eval(x + chunk->renderPos.x, z + chunk->renderPos.z) + 1.f) * 3.14159265359f;
				float dx = 0;// cos(a) * 1;
				float dz = 0;// sin(a) * 1;

				float n = sg.Fractal(x + chunk->renderPos.x + dx, z + chunk->renderPos.z + dz);
				n = (n + 1) * 0.5f;
				n = zm::Clamp(n, 0.f, 1.f);
				int h = zm::Floor(n * 64);

				//int h = zm::Max(zm::Abs(x + chunk->renderPos.x), zm::Abs(z + chunk->renderPos.z)) & 255;

				for (int y = 0; y <= h; y++)
					chunk->SetBlockLocal(x, y, z, 2);
				for (int y = h + 1; y < Chunk::CHUNK_HEIGHT; y++)
					chunk->SetBlockLocal(x, y, z, 0);
			}
		}
	}
}