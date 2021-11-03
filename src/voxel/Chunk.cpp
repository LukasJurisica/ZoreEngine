#include "voxel/Chunk.hpp"
#include "math/WhiteNoise.hpp"
#include "debug/Debug.hpp"

#include "utils/Timer.hpp"

namespace zore {

	FaceData::FaceData(uint x, uint y, uint z, uint blockID, uint ao, uint dir) {
		// 6 bits for each of x, y, z. This leaves 14 bits in value.x
		value.x = (x << 26) | (y << 20) | (z << 14);
		// 16 bits blockID, 8 bits AO, 3 bits dir. This leaves 5 bits in value.y
		value.y = (blockID << 16) | (ao << 8) | (dir);
	}



	//========================================================================
	// Chunk Class
	//========================================================================

	Chunk::Chunk(int x, int z) :
		mesh(nullptr), faceCount(0), chunkPos(x, z), renderPos(x * CHUNK_SIZE, 0, z * CHUNK_SIZE),
		blockData{}, neighbours{}, numNeighbours(1), state(State::INITIATED) {
		neighbours[4] = this;
	}

	Chunk::~Chunk() {
		delete mesh;
	}

	void Chunk::Generate() {
		float hcs = CHUNK_SIZE * 0.5f;
		float off = zm::Ceil(hcs) - 0.5f;
		float radsqrd = hcs * hcs;
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				for (int z = 0; z < CHUNK_SIZE; z++) {
					//if ((x - off) * (x - off) + (y - off) * (y - off) + (z - off) * (z - off) <= radsqrd)
					//	SetBlockLocal(x, y, z, zm::Floor(zm::WhiteNoise::Eval1(glm::vec3(x, y, z)) * 6) + 1);
					//else
					//	SetBlockLocal(x, y, z, 0);
					if (y > zm::Max(zm::Abs(chunkPos.x), zm::Abs(chunkPos.y)))
					//if (y > zm::Floor(zm::WhiteNoise::Eval1(glm::vec2(x, z)) * 16) + 1)
						SetBlockLocal(x, y, z, 0);
					else
						SetBlockLocal(x, y, z, 2);
				}
			}
		}
	}

	void Chunk::Mesh() {
		faceCount = 0;
		faceData.clear();

		for (uint x = 0; x < CHUNK_SIZE; x++) {
			for (uint y = 0; y < CHUNK_SIZE; y++) {
				for (uint z = 0; z < CHUNK_SIZE; z++) {
					uint blockID = GetBlockLocal(x, y, z);
					if (!blockID)
						continue;

					ubyte ao;
					if (!GetOpaque(x - 1, y, z)) { // WEST
						ao = GetAO(GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x - 1, y + 1, z), GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x - 1, y, z + 1),
							       GetOpaque(x - 1, y - 1, z + 1), GetOpaque(x - 1, y - 1, z), GetOpaque(x - 1, y - 1, z - 1), GetOpaque(x - 1, y, z - 1));
						faceData.emplace_back(x, y, z, blockID, ao, 0);
					}
					if (!GetOpaque(x + 1, y, z)) { // EAST
						ao = GetAO(GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x + 1, y + 1, z), GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x + 1, y, z - 1),
							       GetOpaque(x + 1, y - 1, z - 1), GetOpaque(x + 1, y - 1, z), GetOpaque(x + 1, y - 1, z + 1), GetOpaque(x + 1, y, z + 1));
						faceData.emplace_back(x, y, z, blockID, ao, 1);
					}
					if (!GetOpaque(x, y - 1, z)) { // DOWN
						ao = GetAO(GetOpaque(x - 1, y - 1, z + 1), GetOpaque(x, y - 1, z + 1), GetOpaque(x + 1, y - 1, z + 1), GetOpaque(x + 1, y - 1, z),
							       GetOpaque(x + 1, y - 1, z - 1), GetOpaque(x, y - 1, z - 1), GetOpaque(x - 1, y - 1, z - 1), GetOpaque(x - 1, y - 1, z));
						faceData.emplace_back(x, y, z, blockID, ao, 2);
					}
					if (!GetOpaque(x, y + 1, z)) { // UP
						ao = GetAO(GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x, y + 1, z - 1), GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x + 1, y + 1, z),
							       GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x, y + 1, z + 1), GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x - 1, y + 1, z));
						faceData.emplace_back(x, y, z, blockID, ao, 3);
					}
					if (!GetOpaque(x, y, z - 1)) { // NORTH
						ao = GetAO(GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x, y + 1, z - 1), GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x - 1, y, z - 1),
							       GetOpaque(x - 1, y - 1, z - 1), GetOpaque(x, y - 1, z - 1), GetOpaque(x + 1, y - 1, z - 1), GetOpaque(x + 1, y, z - 1));
						faceData.emplace_back(x, y, z, blockID, ao, 4);
					}
					if (!GetOpaque(x, y, z + 1)) { // SOUTH
						ao = GetAO(GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x, y + 1, z + 1), GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x + 1, y, z + 1),
							       GetOpaque(x + 1, y - 1, z + 1), GetOpaque(x, y - 1, z + 1), GetOpaque(x - 1, y - 1, z + 1), GetOpaque(x - 1, y, z + 1));
						faceData.emplace_back(x, y, z, blockID, ao, 5);
					}
				}
			}
		}
	}

	void Chunk::MeshInternal() {
		faceData.clear();

		for (uint x = 1; x < CHUNK_SIZE - 1; x++) {
			for (uint y = 1; y < CHUNK_SIZE - 1; y++) {
				for (uint z = 1; z < CHUNK_SIZE - 1; z++) {
					uint blockID = GetBlockLocal(x, y, z);
					if (!blockID)
						continue;

					ubyte ao;
					if (!GetOpaqueLocal(x - 1, y, z)) { // WEST
						ao = GetAO(GetOpaqueLocal(x - 1, y + 1, z - 1), GetOpaqueLocal(x - 1, y + 1, z), GetOpaqueLocal(x - 1, y + 1, z + 1), GetOpaqueLocal(x - 1, y, z + 1),
							GetOpaqueLocal(x - 1, y - 1, z + 1), GetOpaqueLocal(x - 1, y - 1, z), GetOpaqueLocal(x - 1, y - 1, z - 1), GetOpaqueLocal(x - 1, y, z - 1));
						faceData.emplace_back(x, y, z, blockID, ao, 0);
					}
					if (!GetOpaqueLocal(x + 1, y, z)) { // EAST
						ao = GetAO(GetOpaqueLocal(x + 1, y + 1, z + 1), GetOpaqueLocal(x + 1, y + 1, z), GetOpaqueLocal(x + 1, y + 1, z - 1), GetOpaqueLocal(x + 1, y, z - 1),
							GetOpaqueLocal(x + 1, y - 1, z - 1), GetOpaqueLocal(x + 1, y - 1, z), GetOpaqueLocal(x + 1, y - 1, z + 1), GetOpaqueLocal(x + 1, y, z + 1));
						faceData.emplace_back(x, y, z, blockID, ao, 1);
					}
					if (!GetOpaqueLocal(x, y - 1, z)) { // DOWN
						ao = GetAO(GetOpaqueLocal(x - 1, y - 1, z + 1), GetOpaqueLocal(x, y - 1, z + 1), GetOpaqueLocal(x + 1, y - 1, z + 1), GetOpaqueLocal(x + 1, y - 1, z),
							GetOpaqueLocal(x + 1, y - 1, z - 1), GetOpaqueLocal(x, y - 1, z - 1), GetOpaqueLocal(x - 1, y - 1, z - 1), GetOpaqueLocal(x - 1, y - 1, z));
						faceData.emplace_back(x, y, z, blockID, ao, 2);
					}
					if (!GetOpaqueLocal(x, y + 1, z)) { // UP
						ao = GetAO(GetOpaqueLocal(x - 1, y + 1, z - 1), GetOpaqueLocal(x, y + 1, z - 1), GetOpaqueLocal(x + 1, y + 1, z - 1), GetOpaqueLocal(x + 1, y + 1, z),
							GetOpaqueLocal(x + 1, y + 1, z + 1), GetOpaqueLocal(x, y + 1, z + 1), GetOpaqueLocal(x - 1, y + 1, z + 1), GetOpaqueLocal(x - 1, y + 1, z));
						faceData.emplace_back(x, y, z, blockID, ao, 3);
					}
					if (!GetOpaqueLocal(x, y, z - 1)) { // NORTH
						ao = GetAO(GetOpaqueLocal(x + 1, y + 1, z - 1), GetOpaqueLocal(x, y + 1, z - 1), GetOpaqueLocal(x - 1, y + 1, z - 1), GetOpaqueLocal(x - 1, y, z - 1),
							GetOpaqueLocal(x - 1, y - 1, z - 1), GetOpaqueLocal(x, y - 1, z - 1), GetOpaqueLocal(x + 1, y - 1, z - 1), GetOpaqueLocal(x + 1, y, z - 1));
						faceData.emplace_back(x, y, z, blockID, ao, 4);
					}
					if (!GetOpaqueLocal(x, y, z + 1)) { // SOUTH
						ao = GetAO(GetOpaqueLocal(x - 1, y + 1, z + 1), GetOpaqueLocal(x, y + 1, z + 1), GetOpaqueLocal(x + 1, y + 1, z + 1), GetOpaqueLocal(x + 1, y, z + 1),
							GetOpaqueLocal(x + 1, y - 1, z + 1), GetOpaqueLocal(x, y - 1, z + 1), GetOpaqueLocal(x - 1, y - 1, z + 1), GetOpaqueLocal(x - 1, y, z + 1));
						faceData.emplace_back(x, y, z, blockID, ao, 5);
					}
				}
			}
		}
	}

	bool Chunk::ShouldBeDrawn(const Camera& camera) const {
		return faceCount > 0 && camera.TestAABB(renderPos, glm::vec3(Chunk::CHUNK_SIZE));
	}

	bool Chunk::CanBeMeshed() const {
		if (numNeighbours != 9)
			return false;
		for (int i = 0; i < 9; i++)
			if (neighbours[i]->state < State::GENERATED)
				return false;
		return true;
	}

	unsigned int Chunk::Bind() const {
		mesh->Bind();
		return faceCount;
	}

	void Chunk::UploadMesh() {
		faceCount = static_cast<uint>(faceData.size());
		if (mesh)
			mesh->Set(faceData.data(), sizeof(FaceData) * faceCount, sizeof(FaceData));
		else
			mesh = InstanceArrayBuffer::Create(faceData.data(), sizeof(FaceData) * faceCount, sizeof(FaceData));
	}

	//------------------------------------------------------------------------
	//	Chunk Data Accessor Functions
	//------------------------------------------------------------------------

	void Chunk::SetBlock(int x, int y, int z, ushort value) {
		if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE)
		blockData[(x * CHUNK_SIZE_SQRD) + (y * CHUNK_SIZE) + z] = value;
	}

	ushort Chunk::GetBlock(int x, int y, int z) {
		if (y < 0)
			return 1;
		else if (y >= CHUNK_HEIGHT)
			return 0;
		else if (x < 0 || z < 0 || x >= CHUNK_SIZE || z >= CHUNK_SIZE)
			return 0;
		return blockData[(x * CHUNK_SIZE_SQRD) + (y * CHUNK_SIZE) + z];
	}

	const glm::ivec3& Chunk::GetPosition() {
		return renderPos;
	}

	void Chunk::SetBlockLocal(ubyte x, ubyte y, ubyte z, ushort value) {
		blockData[(x * CHUNK_SIZE_SQRD) + (y * CHUNK_SIZE) + z] = value;
	}

	ushort Chunk::GetBlockLocal(ubyte x, ubyte y, ubyte z) {
		return blockData[(x * CHUNK_SIZE_SQRD) + (y * CHUNK_SIZE) + z];
	}

	bool Chunk::GetOpaque(int x, int y, int z) {
		if (y < 0)
			return 1;
		else if (y >= CHUNK_HEIGHT)
			return 0;

		int dx = (x + CHUNK_SIZE) / CHUNK_SIZE - 1;
		int dz = (z + CHUNK_SIZE) / CHUNK_SIZE - 1;

		if (dx || dz) {
			x -= dx * CHUNK_SIZE;
			z -= dz * CHUNK_SIZE;
			return neighbours[(dx * 3) + dz + 4]->blockData[(x * CHUNK_SIZE_SQRD) + (y * CHUNK_SIZE) + z];
		}
		else
			return blockData[(x * CHUNK_SIZE_SQRD) + (y * CHUNK_SIZE) + z];
	}

	bool Chunk::GetOpaqueLocal(ubyte x, ubyte y, ubyte z) {
		return blockData[(x * CHUNK_SIZE_SQRD) + (y * CHUNK_SIZE) + z];
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