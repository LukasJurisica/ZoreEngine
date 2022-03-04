#include "voxel/Chunk.hpp"
#include "debug/Debug.hpp"
#include "utils/Timer.hpp"

#include "math/WhiteNoise.hpp"
#include "math/MathUtils.hpp"

namespace zore {

	//========================================================================
	// Chunk Meshing Data Structures
	//========================================================================

	FaceData::FaceData(uint x, uint y, uint z, uint blockID, uint ao, uint dir) {
		// 6 bits for x, 9 bits for y, 6 bits for z. This leaves 11 bits in a
		a = (x << 26) | (y << 17) | (z << 11);
		// 16 bits blockID, 8 bits AO, 3 bits dir. This leaves 5 bits in b
		b = ((blockID & ID_MASK) << 16) | (ao << 8) | (dir);
	}

	ChunkMesh::ChunkMesh() : buffer(nullptr), count(0) {}

	ChunkMesh::~ChunkMesh() { delete buffer; }

	void ChunkMesh::Reset() {
		faces.clear();
	}

	void ChunkMesh::Upload() {
		count = static_cast<uint>(faces.size());
		if (count == 0)
			return;
		else if (buffer)
			buffer->Set(faces.data(), sizeof(FaceData) * count, sizeof(FaceData));
		else
			buffer = InstanceArrayBuffer::Create(faces.data(), sizeof(FaceData) * count, sizeof(FaceData));
	}

	unsigned int ChunkMesh::Bind() const {
		if (count > 0)
			buffer->Bind();
		return count;
	}

	//========================================================================
	// Chunk Class
	//========================================================================

	Chunk::Chunk(int x, int z) :
		chunkPos(x, z), renderPos(x * CHUNK_WIDTH, 0, z * CHUNK_WIDTH),
		blockData(nullptr), neighbours{}, numNeighbours(1), state(State::INITIATED) {
		neighbours[4] = this;
	}

	Chunk::~Chunk() {
		delete[] blockData;
	}

	void Chunk::Mesh() {
		blockMesh.Reset();
		fluidMesh.Reset();
		spriteMesh.Reset();

		for (uint x = 0; x < CHUNK_WIDTH; x++) {
			for (uint z = 0; z < CHUNK_WIDTH; z++) {
				for (uint y = 0; y < CHUNK_HEIGHT; y++) {
					uint blockID = GetBlockLocal(x, y, z);
					if (!blockID)
						continue;
					else if (blockID & BLOCK_BIT)
						AddBlock(blockID, x, y, z);
					else if (blockID & FLUID_BIT)
						AddFluid(blockID, x, y, z);
					else
						AddSprite(blockID, x, y, z);
				}
			}
		}
	}

	bool Chunk::CanBeMeshed() const {
		for (int i = 0; i < 9; i++)
			if (neighbours[i]->state < State::GENERATED)
				return false;
		return true;
	}

	//------------------------------------------------------------------------
	//	Chunk Data Accessor Functions
	//------------------------------------------------------------------------

	void Chunk::SetBlock(int x, int y, int z, ushort value) {
		if (y < 0 || y >= CHUNK_HEIGHT)
			return;

		DEBUG_ENSURE(x >= -CHUNK_WIDTH && x < CHUNK_WIDTH * 2 && z >= -CHUNK_WIDTH && z < CHUNK_WIDTH * 2,
			"Chunk::SetBlock only accepts coordinates that are within the moore neighbourhood range (only directly neighbouring chunks)");

		int dx = x >> CHUNK_WIDTH_BIT_DEPTH;
		int dz = z >> CHUNK_WIDTH_BIT_DEPTH;

		if (dx || dz) {
			Chunk* neighbour = neighbours[(dx * 3) + dz + 4];
			if (neighbour)
				neighbour->blockData[((x - dx * CHUNK_WIDTH) * CHUNK_SLICE) + ((z - dz * CHUNK_WIDTH) * CHUNK_HEIGHT) + y] = value;
		}
		else
			blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y] = value;
	}

	ushort Chunk::GetBlock(int x, int y, int z) {
		if (y < 0)
			return BLOCK_STONE;
		else if (y >= CHUNK_HEIGHT)
			return BLOCK_AIR;
		
		DEBUG_ENSURE(x >= -CHUNK_WIDTH && x < CHUNK_WIDTH*2 && z >= -CHUNK_WIDTH && z < CHUNK_WIDTH * 2,
			"Chunk::GetBlock only accepts coordinates that are within the moore neighbourhood range (only directly neighbouring chunks)");

		int dx = x >> CHUNK_WIDTH_BIT_DEPTH;
		int dz = z >> CHUNK_WIDTH_BIT_DEPTH;

		if (dx || dz) {
			if (neighbours[(dx * 3) + dz + 4])
				return neighbours[(dx * 3) + dz + 4]->blockData[((x - dx * CHUNK_WIDTH) * CHUNK_SLICE) + ((z - dz * CHUNK_WIDTH) * CHUNK_HEIGHT) + y];
			return BLOCK_AIR;
		}
		return blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y];
	}

	const glm::ivec3& Chunk::GetPosition() const {
		return renderPos;
	}

	const ChunkMesh& Chunk::GetChunkMesh(ChunkMeshType type) const {
		switch (type) {
		case ChunkMeshType::BLOCKS:
			return blockMesh;
		case ChunkMeshType::FLUIDS:
			return fluidMesh;
		case ChunkMeshType::SPRITES:
		default:
			return spriteMesh;
		}
	}

	Chunk* Chunk::GetNeighbour(int dx, int dz) {
		Chunk* result = this;
		while (dx || dz) {
			int sx = zm::Sign(dx);
			int sz = zm::Sign(dz);
			result = result->neighbours[(sx * 3) + sz + 4];
			dx -= sx;
			dz -= sz;
		}
		return result;
	}

	bool Chunk::GetOpaque(int x, int y, int z) {
		if (y < 0)
			return true;
		else if (y >= CHUNK_HEIGHT)
			return false;

		DEBUG_ENSURE(x >= -CHUNK_WIDTH && x < CHUNK_WIDTH * 2 && z >= -CHUNK_WIDTH && z < CHUNK_WIDTH * 2,
			"Chunk::GetOpaque only accepts coordinates that are within the moore neighbourhood range (only directly neighbouring chunks)");

		int dx = x >> CHUNK_WIDTH_BIT_DEPTH;
		int dz = z >> CHUNK_WIDTH_BIT_DEPTH;

		ushort block;
		if (dx || dz)
			block = neighbours[(dx * 3) + dz + 4]->blockData[((x - dx * CHUNK_WIDTH) * CHUNK_SLICE) + ((z - dz * CHUNK_WIDTH) * CHUNK_HEIGHT) + y];
		else
			block = blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y];
		return (block & OPAQUE_MASK) == OPAQUE_MASK;
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

	void Chunk::AddBlock(ushort block, ubyte x, ubyte y, ubyte z) {
		ubyte ao;
		if (!GetOpaque(x - 1, y, z)) { // WEST
			ao = GetAO(GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x - 1, y + 1, z), GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x - 1, y, z + 1),
			           GetOpaque(x - 1, y - 1, z + 1), GetOpaque(x - 1, y - 1, z), GetOpaque(x - 1, y - 1, z - 1), GetOpaque(x - 1, y, z - 1));
			blockMesh.faces.emplace_back(x, y, z, block, ao, 0);
		}
		if (!GetOpaque(x + 1, y, z)) { // EAST
			ao = GetAO(GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x + 1, y + 1, z), GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x + 1, y, z - 1),
			           GetOpaque(x + 1, y - 1, z - 1), GetOpaque(x + 1, y - 1, z), GetOpaque(x + 1, y - 1, z + 1), GetOpaque(x + 1, y, z + 1));
			blockMesh.faces.emplace_back(x, y, z, block, ao, 1);
		}
		if (!GetOpaque(x, y - 1, z)) { // DOWN
			ao = GetAO(GetOpaque(x - 1, y - 1, z + 1), GetOpaque(x, y - 1, z + 1), GetOpaque(x + 1, y - 1, z + 1), GetOpaque(x + 1, y - 1, z),
			           GetOpaque(x + 1, y - 1, z - 1), GetOpaque(x, y - 1, z - 1), GetOpaque(x - 1, y - 1, z - 1), GetOpaque(x - 1, y - 1, z));
			blockMesh.faces.emplace_back(x, y, z, block, ao, 2);
		}
		if (!GetOpaque(x, y + 1, z)) { // UP
			ao = GetAO(GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x, y + 1, z - 1), GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x + 1, y + 1, z),
			           GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x, y + 1, z + 1), GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x - 1, y + 1, z));
			blockMesh.faces.emplace_back(x, y, z, block, ao, 3);
		}
		if (!GetOpaque(x, y, z - 1)) { // NORTH
			ao = GetAO(GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x, y + 1, z - 1), GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x - 1, y, z - 1),
			           GetOpaque(x - 1, y - 1, z - 1), GetOpaque(x, y - 1, z - 1), GetOpaque(x + 1, y - 1, z - 1), GetOpaque(x + 1, y, z - 1));
			blockMesh.faces.emplace_back(x, y, z, block, ao, 4);
		}
		if (!GetOpaque(x, y, z + 1)) { // SOUTH
			ao = GetAO(GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x, y + 1, z + 1), GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x + 1, y, z + 1),
			           GetOpaque(x + 1, y - 1, z + 1), GetOpaque(x, y - 1, z + 1), GetOpaque(x - 1, y - 1, z + 1), GetOpaque(x - 1, y, z + 1));
			blockMesh.faces.emplace_back(x, y, z, block, ao, 5);
		}
	}
	
	void Chunk::AddFluid(ushort fluid, ubyte x, ubyte y, ubyte z) {
		ubyte ao;
		if (GetBlock(x, y + 1, z) != fluid) { // UP
			ao = GetAO(GetOpaque(x - 1, y + 1, z - 1), GetOpaque(x, y + 1, z - 1), GetOpaque(x + 1, y + 1, z - 1), GetOpaque(x + 1, y + 1, z),
			           GetOpaque(x + 1, y + 1, z + 1), GetOpaque(x, y + 1, z + 1), GetOpaque(x - 1, y + 1, z + 1), GetOpaque(x - 1, y + 1, z));
			fluidMesh.faces.emplace_back(x, y, z, fluid, 0, 3);
		}
	}

	void Chunk::AddSprite(ushort sprite, ubyte x, ubyte y, ubyte z) {
		spriteMesh.faces.emplace_back(x, y, z, sprite, 0, 6);
		spriteMesh.faces.emplace_back(x, y, z, sprite, 0, 7);
	}
}