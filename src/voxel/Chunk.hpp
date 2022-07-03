#pragma once
#include "graphics/Buffer.hpp"
#include "voxel/Block.hpp"

#include <glm/ext/vector_int3.hpp>
#include <glm/ext/vector_int2.hpp>
#include <vector>

namespace zore {

	class ChunkColumn;
	enum class BlockFace { EAST, WEST, UP, DOWN, NORTH, SOUTH };

	struct FaceData {
		FaceData(uint x, uint y, uint z, uint blockID, uint ao, uint dir);
		FaceData(const FaceData&) = delete; // This can be removed once I fix the chunk class and chunk column class
		FaceData(FaceData&&) = default;
		uint a, b;
	};

	struct ChunkMesh {
		ChunkMesh();
		~ChunkMesh();
		void Reset();
		void Upload();
		unsigned int Bind() const;
		
		std::vector<FaceData> faces;
		InstanceArrayBuffer* buffer;
		uint count;
	};
	enum class ChunkMeshType { BLOCKS, FLUIDS, SPRITES };

	//========================================================================
	// Chunk Class
	//========================================================================

	class Chunk {
		friend class ChunkManager;
		friend class TerrainGenerator;
	public:
		Chunk(int x, int z);
		~Chunk();

		void Mesh();
		bool CanBeMeshed() const;

		void SetBlock(int x, int y, int z, ushort value);
		ushort GetBlock(int x, int y, int z);
		const glm::ivec3& GetPosition() const;
		const ChunkMesh& GetChunkMesh(ChunkMeshType type) const;
		Chunk* GetNeighbour(int dx, int dz);

		static constexpr int CHUNK_WIDTH = 64; // Chunk width and height must be powers of 2
		static constexpr int CHUNK_HALF_WIDTH = CHUNK_WIDTH / 2;
		static constexpr int CHUNK_HEIGHT = 256;
		static constexpr int CHUNK_HSLICE = CHUNK_WIDTH * CHUNK_WIDTH;
		static constexpr int CHUNK_VSLICE = CHUNK_WIDTH * CHUNK_HEIGHT;
		static constexpr int CHUNK_VOLUME = CHUNK_VSLICE * CHUNK_WIDTH;
		static constexpr int CHUNK_WIDTH_BIT_DEPTH = 6; // This needs to be updated manually to reflect chunk width
		static constexpr int CHUNK_HEIGHT_BIT_DEPTH = 8; // This needs to be updated manually to reflect chunk height
		enum class State { INITIATED, GENERATING, GENERATED, MESHING, MESHED };

	private:
		// Special Access for gauranteed local operations (slightly faster than non local/relative counterparts). It is safer to use the public version 
		inline void SetBlockLocal(int x, int y, int z, ushort value) { blockData[(x * CHUNK_VSLICE) + (z * CHUNK_HEIGHT) + y] = value; }
		inline ushort GetBlockLocal(int x, int y, int z) { return blockData[(x * CHUNK_VSLICE) + (z * CHUNK_HEIGHT) + y]; }
		inline bool GetOpaqueLocal(int x, int y, int z) { return blockData[(x * CHUNK_VSLICE) + (z * CHUNK_HEIGHT) + y]; }

		// Functions for Meshing
		inline bool GetOpaque(int x, int y, int z);
		inline ubyte GetAO(ubyte ao1, ubyte ao2, ubyte ao3, ubyte ao4, ubyte ao5, ubyte ao6, ubyte ao7, ubyte ao8);
		inline void AddBlock(ushort block, ubyte x, ubyte y, ubyte z);
		inline void AddFluid(ushort fluid, ubyte x, ubyte y, ubyte z);
		inline void AddSprite(ushort sprite, ubyte x, ubyte y, ubyte z);

		// Data Members
		ushort* blockData;
		Chunk* neighbours[9];
		ubyte numNeighbours;

		ChunkMesh blockMesh;
		ChunkMesh fluidMesh;
		ChunkMesh spriteMesh;

		glm::ivec3 renderPos;
		glm::ivec2 chunkPos;
		State state;
	};
}