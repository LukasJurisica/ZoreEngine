#pragma once
#include "utils/DataTypes.hpp"
#include "graphics/Buffer.hpp"
#include "core/Camera.hpp"

#include <glm/ext/vector_int3.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <vector>

namespace zore {

	class ChunkColumn;
	enum class BlockFace { EAST, WEST, UP, DOWN, NORTH, SOUTH };

	struct FaceData {
		FaceData(uint x, uint y, uint z, uint blockID, uint ao, uint dir);
		FaceData(const FaceData&) = delete; // This can be removed once I fix the chunk class and chunk column class
		FaceData(FaceData&&) = default;
		glm::uvec2 value;
	};

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
		bool ShouldBeDrawn(const Camera& camera) const;
		bool CanBeMeshed() const;
		unsigned int Bind() const;
		void UploadMesh();

		void SetBlock(int x, int y, int z, ushort value);
		ushort GetBlock(int x, int y, int z);
		const glm::ivec3& GetPosition();

		static constexpr int CHUNK_WIDTH = 64;
		static constexpr int CHUNK_HALF_WIDTH = CHUNK_WIDTH / 2;
		static constexpr int CHUNK_HEIGHT = 256;
		static constexpr int CHUNK_SLICE = CHUNK_WIDTH * CHUNK_HEIGHT;
		static constexpr int CHUNK_VOLUME = CHUNK_SLICE * CHUNK_WIDTH;
		enum class State { INITIATED, GENERATING, GENERATED, MESHING, MESHED };

	private:
		// Special Access for gauranteed local operations (slightly faster than non local/relative counterparts). It is safer to use the public version 
		inline void SetBlockLocal(int x, int y, int z, ushort value) { blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y] = value; }
		inline ushort GetBlockLocal(int x, int y, int z) { return blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y]; }
		inline bool GetOpaqueLocal(int x, int y, int z) { return blockData[(x * CHUNK_SLICE) + (z * CHUNK_HEIGHT) + y]; }

		// Functions for Meshing
		inline bool GetOpaque(int x, int y, int z);
		inline ubyte GetAO(ubyte ao1, ubyte ao2, ubyte ao3, ubyte ao4, ubyte ao5, ubyte ao6, ubyte ao7, ubyte ao8);

		// Data Members
		ushort* blockData;
		Chunk* neighbours[9];
		ubyte numNeighbours;

		uint terrainFaceCount;
		std::vector<FaceData> terrainFaceData;
		InstanceArrayBuffer* terrainMesh;

		glm::ivec3 renderPos;
		glm::ivec2 chunkPos;
		State state;
	};
}