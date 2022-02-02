#pragma once
#include "voxel/Chunk.hpp"
#include "core/Camera.hpp"
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
		static void Init(uint renderDistance, const glm::vec3& position);
		static void Cleanup();

		static void Update(Camera* camera);
		static void RemeshChunk(Chunk* chunk, int xNeighbour, int zNeighbour);
		static Chunk* GetChunk(int x, int z);
		static Chunk* GetChunk(const glm::vec3& position);
		static glm::ivec3 GetChunkCoord(const glm::vec3& position);
		static const std::unordered_map<size_t, Chunk*>& GetChunks();
		static const std::vector<Chunk*>& GetVisibleChunks();

	private:
		static void UpdateChunks();
		static void LoadChunks();
		static void UnloadChunks();

		static size_t GetChunkKey(int x, int z);
		static bool CoordInRenderRange(int x, int z);
		static Chunk* EnsureChunk(int x, int z);
		static void LoadNeighbours(Chunk* chunk);
		static bool UnloadNeighbours(Chunk* chunk);

		static void ThreadLoop();
		static Job GetNextJob();
		static void SortJobs();
		static int GetDistanceToJob(const Job& job);
	};
}