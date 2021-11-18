#include "voxel/Chunk.hpp"
#include "debug/Debug.hpp"
#include "utils/Timer.hpp"

#include "math/WhiteNoise.hpp"
#include "math/MathUtils.hpp"

namespace zore {

	FaceData::FaceData(uint x, uint y, uint z, uint blockID, uint ao, uint dir) {
		// 6 bits for x, 9 bits for y, 6 bits for z. This leaves 11 bits in value.x
		value.x = (x << 26) | (y << 17) | (z << 11);
		// 16 bits blockID, 8 bits AO, 3 bits dir. This leaves 5 bits in value.y
		value.y = (blockID << 16) | (ao << 8) | (dir);
	}

	//========================================================================
	// Chunk Class
	//========================================================================

	Chunk::Chunk(int x, int z) :
		terrainMesh(nullptr), terrainFaceCount(0), chunkPos(x, z), renderPos(x * CHUNK_WIDTH, 0, z * CHUNK_WIDTH),
		blockData(nullptr), neighbours{}, numNeighbours(1), state(State::INITIATED) {
		neighbours[4] = this;
	}

	Chunk::~Chunk() {
		delete terrainMesh;
		delete[] blockData;
	}

	void Chunk::Mesh() {
		terrainFaceCount = 0;
		terrainFaceData.clear();

		for (uint x = 0; x < CHUNK_WIDTH; x++) {
			for (uint z = 0; z < CHUNK_WIDTH; z++) {
				for (uint y = 0; y < CHUNK_HEIGHT; y++) {
					uint blockID = GetBlockLocal(x, y, z);
					if (!blockID)
						continue;

					ubyte ao;
					if (!GetOpaque(x - 1, y, z)) { // WEST
						ao = GetAO(GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x - 1, y + 1, z), GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x - 1, y, z + 1),
							       GetOpaque(x - 1, y - 1, z + 1), GetOpaque(x - 1, y - 1, z), GetOpaque(x - 1, y - 1, z - 1), GetOpaque(x - 1, y, z - 1));
						terrainFaceData.emplace_back(x, y, z, blockID, ao, 0);
					}
					if (!GetOpaque(x + 1, y, z)) { // EAST
						ao = GetAO(GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x + 1, y + 1, z), GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x + 1, y, z - 1),
							       GetOpaque(x + 1, y - 1, z - 1), GetOpaque(x + 1, y - 1, z), GetOpaque(x + 1, y - 1, z + 1), GetOpaque(x + 1, y, z + 1));
						terrainFaceData.emplace_back(x, y, z, blockID, ao, 1);
					}
					if (!GetOpaque(x, y - 1, z)) { // DOWN
						ao = GetAO(GetOpaque(x - 1, y - 1, z + 1), GetOpaque(x, y - 1, z + 1), GetOpaque(x + 1, y - 1, z + 1), GetOpaque(x + 1, y - 1, z),
							       GetOpaque(x + 1, y - 1, z - 1), GetOpaque(x, y - 1, z - 1), GetOpaque(x - 1, y - 1, z - 1), GetOpaque(x - 1, y - 1, z));
						terrainFaceData.emplace_back(x, y, z, blockID, ao, 2);
					}
					if (!GetOpaque(x, y + 1, z)) { // UP
						ao = GetAO(GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x, y + 1, z - 1), GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x + 1, y + 1, z),
							       GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x, y + 1, z + 1), GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x - 1, y + 1, z));
						terrainFaceData.emplace_back(x, y, z, blockID, ao, 3);
					}
					if (!GetOpaque(x, y, z - 1)) { // NORTH
						ao = GetAO(GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x, y + 1, z - 1), GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x - 1, y, z - 1),
							       GetOpaque(x - 1, y - 1, z - 1), GetOpaque(x, y - 1, z - 1), GetOpaque(x + 1, y - 1, z - 1), GetOpaque(x + 1, y, z - 1));
						terrainFaceData.emplace_back(x, y, z, blockID, ao, 4);
					}
					if (!GetOpaque(x, y, z + 1)) { // SOUTH
						ao = GetAO(GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x, y + 1, z + 1), GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x + 1, y, z + 1),
							       GetOpaque(x + 1, y - 1, z + 1), GetOpaque(x, y - 1, z + 1), GetOpaque(x - 1, y - 1, z + 1), GetOpaque(x - 1, y, z + 1));
						terrainFaceData.emplace_back(x, y, z, blockID, ao, 5);
					}
				}
			}
		}
	}

	bool Chunk::ShouldBeDrawn(const Camera& camera) const {
		return terrainFaceCount > 0 && camera.TestAABB(renderPos, glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH));
	}

	bool Chunk::CanBeMeshed() const {
		for (int i = 0; i < 9; i++)
			if (neighbours[i]->state < State::GENERATED)
				return false;
		return true;
	}

	unsigned int Chunk::Bind() const {
		terrainMesh->Bind();
		return terrainFaceCount;
	}

	void Chunk::UploadMesh() {
		terrainFaceCount = static_cast<uint>(terrainFaceData.size());
		if (terrainMesh)
			terrainMesh->Set(terrainFaceData.data(), sizeof(FaceData) * terrainFaceCount, sizeof(FaceData));
		else
			terrainMesh = InstanceArrayBuffer::Create(terrainFaceData.data(), sizeof(FaceData) * terrainFaceCount, sizeof(FaceData));
	}

	//------------------------------------------------------------------------
	//	Chunk Data Accessor Functions
	//------------------------------------------------------------------------

	void Chunk::SetBlock(int x, int y, int z, ushort value) {
		if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_WIDTH && y < CHUNK_HEIGHT && z < CHUNK_WIDTH)
		blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y] = value;
	}

	ushort Chunk::GetBlock(int x, int y, int z) {
		if (y < 0)
			return 1;
		else if (y >= CHUNK_HEIGHT)
			return 0;
		else if (x < 0 || z < 0 || x >= CHUNK_WIDTH || z >= CHUNK_WIDTH)
			return 0;
		return blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y];
	}

	const glm::ivec3& Chunk::GetPosition() {
		return renderPos;
	}

	bool Chunk::GetOpaque(int x, int y, int z) {
		if (y < 0)
			return true;
		else if (y >= CHUNK_HEIGHT)
			return false;

		int dx = ((x + CHUNK_WIDTH) / CHUNK_WIDTH) - 1;
		int dz = ((z + CHUNK_WIDTH) / CHUNK_WIDTH) - 1;

		if (dx || dz) {
			x -= dx * CHUNK_WIDTH;
			z -= dz * CHUNK_WIDTH;
			return neighbours[(dx * 3) + dz + 4]->blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y];
		}
		else
			return blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y];
	}

	ubyte Chunk::GetAO(ubyte ao1, ubyte ao2, ubyte ao3, ubyte ao4, ubyte ao5, ubyte ao6, ubyte ao7, ubyte ao8) {		
		//ubyte aoLookupTable[] = { 0, 1, 1, 2, 1, 3, 2, 3 }; // A VERSION
		ubyte aoLookupTable[] = { 0, 1, 1, 1, 1, 2, 1, 2 }; // B VERSION

		// 26 total operations + lookup table memory
		return aoLookupTable[(ao8 << 2) | (ao1 << 1) | (ao2)] << 6 |
		       aoLookupTable[(ao6 << 2) | (ao7 << 1) | (ao8)] << 4 |
			   aoLookupTable[(ao2 << 2) | (ao3 << 1) | (ao4)] << 2 |
			   aoLookupTable[(ao4 << 2) | (ao5 << 1) | (ao6)];
	}
}