#pragma once
#include "voxel/Chunk.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <glm/ext/vector_int2.hpp>
#include <unordered_map>

namespace zore {

	struct Job;

	//========================================================================
	// Chunk Manager Class
	//========================================================================

	class ChunkManager {
	public:
		static void Init(uint renderDistance, const glm::vec3& pos);
		static void Cleanup();
		static void Update(const glm::vec3& pos);

		static Chunk* GetChunk(int x, int z);
		static const std::unordered_map<size_t, Chunk*>& GetChunks();
		static size_t GetChunkKey(int x, int z);
		static glm::ivec3 GetChunkCoord(const glm::vec3& pos);
		static bool CoordInRenderRange(int x, int z);

	private:
		static void UpdateChunks();
		static void LoadChunks();
		static void UnloadChunks();

		static Chunk* EnsureChunk(int x, int z);
		static void LoadNeighbours(Chunk* chunk);
		static bool UnloadNeighbours(Chunk* chunk);

		static void ThreadLoop();
		static Job GetNextJob();
		static void SortJobs();
		static int GetDistanceToJob(const Job& job);
	};
}