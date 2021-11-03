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
	public:
		Chunk(int x, int z);
		~Chunk();

		void Generate();
		void Mesh();
		void MeshInternal();
		bool ShouldBeDrawn(const Camera& camera) const;
		bool CanBeMeshed() const;
		unsigned int Bind() const;
		void UploadMesh();

		void SetBlock(int x, int y, int z, ushort value);
		ushort GetBlock(int x, int y, int z);
		const glm::ivec3& GetPosition();

		static constexpr int CHUNK_SIZE = 64;
		static constexpr int CHUNK_HEIGHT = 64;
		static constexpr int CHUNK_SIZE_SQRD = CHUNK_SIZE * CHUNK_SIZE;
		static constexpr int CHUNK_SIZE_CUBD = CHUNK_SIZE_SQRD * CHUNK_SIZE;
		enum class State { INITIATED, GENERATING, GENERATED, MESHING, MESHED };

	private:
		// Special Access for gauranteed local operations (slightly faster than non local/relative counterparts). It is safer to use the public version 
		void SetBlockLocal(ubyte x, ubyte y, ubyte z, ushort value);
		ushort GetBlockLocal(ubyte x, ubyte y, ubyte z);

		// Functions for Meshing
		bool GetOpaque(int x, int y, int z);
		bool GetOpaqueLocal(ubyte x, ubyte y, ubyte z);
		ubyte GetAO(ubyte ao1, ubyte ao2, ubyte ao3, ubyte ao4, ubyte ao5, ubyte ao6, ubyte ao7, ubyte ao8);

		// Data Members
		ushort blockData[CHUNK_SIZE_CUBD];
		Chunk* neighbours[9];
		ubyte numNeighbours;

		uint faceCount;
		std::vector<FaceData> faceData;
		InstanceArrayBuffer* mesh;

		glm::ivec3 renderPos;
		glm::ivec2 chunkPos;
		State state;
	};
}