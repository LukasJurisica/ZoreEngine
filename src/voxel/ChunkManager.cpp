#include "voxel/ChunkManager.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

#include <condition_variable>
#include <thread>
#include <algorithm>

#include <iostream>

namespace zore {

	//========================================================================
	// Chunk Manager Data Members
	//========================================================================

	enum class Task { NONE, GENERATE, MESH };
	struct Job { Task task; Chunk* chunk; };

	const glm::ivec2 moore[] = { {-1,-1}, {-1, 0}, {-1, 1}, { 0,-1}, { 0, 1}, { 1,-1}, { 1, 0}, { 1, 1} };

	std::mutex jobMutex;
	std::vector<Job> jobs;
	std::mutex uploadMutex;
	std::vector<Chunk*> toBeUploaded;

	std::condition_variable cv;
	std::vector<std::thread> threadPool;
	bool running;

	std::unordered_map<size_t, Chunk*> chunks;
	float chunkRadiusSquared;
	glm::ivec2 fulcrum;
	glm::ivec2 quadrant;

	//========================================================================
	// Chunk Manager Class
	//========================================================================

	void ChunkManager::Init(uint renderDistance, const glm::vec3& pos) {
		// Initialize data for chunk management
		int radius = renderDistance + 1;
		chunkRadiusSquared = (radius + 0.5f) * (radius + 0.5f);

		fulcrum = { zm::Floor(pos.x / Chunk::CHUNK_SIZE), zm::Floor(pos.z / Chunk::CHUNK_SIZE) };
		quadrant = { zm::Floor(pos.x) & 32, zm::Floor(pos.z) & 32 };

		// Create chunk columns
		for (int x = -radius; x <= radius; x++) {
			for (int z = -radius; z <= radius; z++) {
				int lx = fulcrum.x + x, lz = fulcrum.y + z;
				if (CoordInRenderRange(lx, lz))
					EnsureChunk(lx, lz);
			}
		}

		SortJobs();

		// Create Threads
		running = true;
		int maxThreads = std::thread::hardware_concurrency();
		int numThreads = 4;
		for (int i = 0; i < numThreads; i++)
			threadPool.emplace_back(&ChunkManager::ThreadLoop);

		Logger::Log("numChunks: " + TOSTR(chunks.size()));
	}

	void ChunkManager::Cleanup() {
		jobMutex.lock();
		running = false;
		jobMutex.unlock();
		cv.notify_all();
		for (std::thread& worker : threadPool)
			worker.join();

		// Cleanup Chunks
		for (std::pair<const size_t, Chunk*>& pair : chunks)
			delete pair.second;
	}

	void ChunkManager::Update(const glm::vec3& pos) {
		uploadMutex.lock();
		for (Chunk* chunk : toBeUploaded)
			chunk->UploadMesh();
		toBeUploaded.clear();
		uploadMutex.unlock();

		// Check if the position crossed a chunk quadrant since last update
		glm::ivec2 newQuadrant = { zm::Floor(pos.x) & 32, zm::Floor(pos.z) & 32 };

		if (quadrant != newQuadrant) {
			quadrant = newQuadrant;

			std::lock_guard<std::mutex> lk(jobMutex);

			// Additionally check if the position crossed a chunk border
			glm::ivec2 newFulcrum = { zm::Floor(pos.x / Chunk::CHUNK_SIZE), zm::Floor(pos.z / Chunk::CHUNK_SIZE) };
			if (fulcrum != newFulcrum) {
				fulcrum = newFulcrum;
				LoadChunks();
				cv.notify_all();
			}
			else {
				UnloadChunks();
			}
		}
	}

	//------------------------------------------------------------------------
	//	Chunk Manager Getter Functions
	//------------------------------------------------------------------------

	Chunk* ChunkManager::GetChunk(int x, int z) {
		std::unordered_map<size_t, Chunk*>::iterator iter = chunks.find(GetChunkKey(x, z));
		return (iter == chunks.end()) ? nullptr : iter->second;
	}

	const std::unordered_map<size_t, Chunk*>& ChunkManager::GetChunks() {
		return chunks;
	}

	size_t ChunkManager::GetChunkKey(int x, int z) {
		size_t result = static_cast<uint>(x);
		return (result << 32) | static_cast<uint>(z);
	}

	glm::ivec3 ChunkManager::GetChunkCoord(const glm::vec3& pos) {
		return {
			zm::Floor(pos.x / Chunk::CHUNK_SIZE),
			zm::Floor(pos.y / Chunk::CHUNK_SIZE),
			zm::Floor(pos.z / Chunk::CHUNK_SIZE)
		};
	}

	bool ChunkManager::CoordInRenderRange(int x, int z) {
		x -= fulcrum.x; z -= fulcrum.y;
		return (x * x) + (z * z) <= chunkRadiusSquared;
	}

	//------------------------------------------------------------------------
	//	Chunk Management Utility Functions
	//------------------------------------------------------------------------

	void ChunkManager::UpdateChunks() {

	}

	void ChunkManager::LoadChunks() {
		std::vector<Chunk*> chunksOfInterest;
		for (std::pair<const size_t, Chunk*>& pair : chunks) {
			Chunk* chunk = pair.second;
			if (chunk->numNeighbours < 9 && CoordInRenderRange(chunk->chunkPos.x, chunk->chunkPos.y))
				chunksOfInterest.emplace_back(chunk);
		}

		for (Chunk* chunk : chunksOfInterest) {
			for (const glm::ivec2& m : moore) {
				int lx = chunk->chunkPos.x + m.x, lz = chunk->chunkPos.y + m.y;
				// This neighbour is not loaded but is in range
				if (!chunk->neighbours[(m.x * 3) + m.y + 4] && CoordInRenderRange(lx, lz))
					EnsureChunk(lx, lz);
			}
		}
	}

	void ChunkManager::UnloadChunks() {
		for (std::unordered_map<const size_t, Chunk*>::iterator iter = chunks.begin(); iter != chunks.end();) {
			Chunk* chunk = iter->second;
			
			if (chunk->numNeighbours < 9 && !CoordInRenderRange(chunk->chunkPos.x, chunk->chunkPos.y)) {
				for (std::vector<Job>::iterator iter = jobs.begin(); iter != jobs.end();)
					(iter->chunk == chunk) ? iter = jobs.erase(iter) : ++iter;

				// All neighbours are ok with unload
				if (UnloadNeighbours(chunk)) {
					delete iter->second;
					iter = chunks.erase(iter);
				}
			}
			// Chunk unload could not be completed
			else
				++iter;
		}
	}

	Chunk* ChunkManager::EnsureChunk(int x, int z) {
		Chunk* chunk = GetChunk(x, z);
		if (!chunk) {
			chunk = new Chunk(x, z);
			chunks.insert({ ChunkManager::GetChunkKey(x, z), chunk });
			LoadNeighbours(chunk);
			jobs.push_back({ Task::GENERATE, chunk });
		}
		return chunk;
	}

	void ChunkManager::LoadNeighbours(Chunk* chunk) {		
		for (const glm::ivec2& m : moore) {
			Chunk* neighbour = ChunkManager::GetChunk(chunk->chunkPos.x + m.x, chunk->chunkPos.y + m.y);
			int index = (m.x * 3) + m.y + 4;
			chunk->neighbours[index] = neighbour;
			if (neighbour) {
				neighbour->neighbours[8 - index] = chunk;
				neighbour->numNeighbours++;
				chunk->numNeighbours++;

				// Potentially remove second operand of if statement (if we want to remesh chunks on neighbour reload)
				if (neighbour->numNeighbours == 9 && neighbour->state < Chunk::State::MESHING)
					jobs.push_back({ Task::MESH, neighbour });
			}
		}
	}

	bool ChunkManager::UnloadNeighbours(Chunk* chunk) {
		bool retVal = true;

		for (const glm::ivec2& m : moore) {
			Chunk* neighbour = chunk->neighbours[(m.x * 3) + m.y + 4];
			if (neighbour) {
				// Not safe to unload
				if (neighbour->state == Chunk::State::MESHING) {
					retVal = false;
					continue;
				}

				// Safe to unload
				neighbour->neighbours[(-m.x * 3) - m.y + 4] = nullptr;
				neighbour->numNeighbours--;
				chunk->numNeighbours--;

				// Remove any meshing jobs related to neighbour
				if (neighbour->state < Chunk::State::MESHED) {
					for (std::vector<Job>::iterator iter = jobs.begin(); iter != jobs.end(); ++iter) {
						if (iter->task == Task::MESH && iter->chunk == neighbour) {
							jobs.erase(iter);
							break;
						}
					}
				}

			}
		}
		return retVal;
	}

	//------------------------------------------------------------------------
	//	Chunk Manager Job/Thread System Functions
	//------------------------------------------------------------------------

	void ChunkManager::ThreadLoop() {
		while (running) {
			Job j = GetNextJob();

			switch (j.task) {
			case Task::GENERATE:
				j.chunk->Generate();
				jobMutex.lock();
				j.chunk->state = Chunk::State::GENERATED;
				jobMutex.unlock();
				break;
			case Task::MESH:
				j.chunk->Mesh();
				jobMutex.lock();
				j.chunk->state = Chunk::State::MESHED;
				jobMutex.unlock();
				uploadMutex.lock();
				toBeUploaded.emplace_back(j.chunk);
				uploadMutex.unlock();
				break;
			}
		}
	}

	Job ChunkManager::GetNextJob() {
		std::unique_lock<std::mutex> lk(jobMutex);
		cv.wait(lk, [] { return jobs.size() > 0 || !running; });

		for (int i = 0; i < jobs.size(); i++) {
			Job j = jobs[i];

			if (j.task == Task::GENERATE)
				j.chunk->state = Chunk::State::GENERATING;
			else if (j.task == Task::MESH) {
				if (j.chunk->CanBeMeshed())
					j.chunk->state = Chunk::State::MESHING;
				else
					continue;
			}

			jobs.erase(jobs.begin() + i);
			return j;
		}
		return { Task::NONE, nullptr };
	}

	void ChunkManager::SortJobs() {
		std::sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
			return GetDistanceToJob(a) < GetDistanceToJob(b);
		});
	}

	int ChunkManager::GetDistanceToJob(const Job& job) {
		glm::ivec3 position = job.chunk->GetPosition() / Chunk::CHUNK_SIZE;
		return zm::SqrDist(position.x, position.z, fulcrum.x, fulcrum.y);
	}
}