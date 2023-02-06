#include "voxel/ChunkManager.hpp"
#include "voxel/ChunkConstants.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

#include <condition_variable>
#include <algorithm>
#include <thread>

namespace zore {

	//========================================================================
	// Chunk Manager Data Members
	//========================================================================

	const glm::ivec2 moore[] = { {-1,-1}, {-1, 0}, {-1, 1}, { 0,-1}, { 0, 1}, { 1,-1}, { 1, 0}, { 1, 1} };
	enum class Task { NONE, GENERATE, MESH };
	struct Job { Task task; zore::Chunk* chunk; };

	std::mutex jobMutex;
	std::vector<Job> jobs;
	std::mutex uploadMutex;
	std::vector<zore::Chunk*> toBeUploaded;
	std::vector<zore::Chunk*> visibleChunks;

	std::condition_variable cv;
	std::vector<std::thread> threadPool;
	bool running;

	std::unordered_map<size_t, zore::Chunk*> chunks;
	float chunkRadiusSquared;
	glm::ivec2 fulcrum;
	glm::ivec2 quadrant;

	void (*generate)(zore::Chunk* chunk) = nullptr;

	//========================================================================
	// Chunk Manager Class
	//========================================================================

	void ChunkManager::Init(uint renderDistance, const glm::vec3& position, void (*GenFunc)(Chunk* chunk)) {
		DEBUG_ENSURE(GenFunc != nullptr, "A valid chunk generation function must be provided before the chunk manager can be initialized");
		generate = GenFunc;
		// Initialize data for chunk management
		int radius = renderDistance + 1;
		chunkRadiusSquared = (radius + 0.5f) * (radius + 0.5f);

		glm::ivec2 fpos = { zm::Floor(position.x), zm::Floor(position.z) };
		fulcrum = { fpos.x >> CHUNK_WIDTH_BIT_DEPTH, fpos.y >> CHUNK_WIDTH_BIT_DEPTH };
		quadrant = { fpos.x & CHUNK_HALF_WIDTH, fpos.y & CHUNK_HALF_WIDTH };

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
		if (!running) {
			running = true;
			int maxThreads = std::thread::hardware_concurrency() - 1;
			int numThreads = 4;
			for (int i = 0; i < numThreads; i++)
				threadPool.emplace_back(&ChunkManager::ThreadLoop);
			Logger::Info("Threads in use for Chunk Generation and Meshing: " + TOSTR(numThreads) + "/" + TOSTR(maxThreads));
			Logger::Info("Number of loaded chunks: " + TOSTR(chunks.size()));
		}
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

	void ChunkManager::SetRenderDistance(uint renderDistance) {
		float oldChunkRadiusSquared = chunkRadiusSquared;
		int radius = renderDistance + 1;
		chunkRadiusSquared = (radius + 0.5f) * (radius + 0.5f);

		// Need to remove chunks that are no longer in range
		if (chunkRadiusSquared < oldChunkRadiusSquared) {
			UnloadChunks();
		}
		// Need to add chunks that are now in range
		else if (chunkRadiusSquared > oldChunkRadiusSquared) {
			for (int x = -radius; x <= radius; x++) {
				for (int z = -radius; z <= radius; z++) {
					int lx = fulcrum.x + x, lz = fulcrum.y + z;
					if (CoordInRenderRange(lx, lz))
						EnsureChunk(lx, lz);
				}
			}
			SortJobs();
			cv.notify_all();
		}
	}

	void ChunkManager::SetGenerationFunction(void (*GenFunc)(Chunk* chunk)) {
		generate = GenFunc;
	}

	void ChunkManager::Update(Camera* camera) {
		uploadMutex.lock();
		for (Chunk* chunk : toBeUploaded) {
			chunk->blockMesh.Upload();
			chunk->fluidMesh.Upload();
			chunk->spriteMesh.Upload();
		}
		toBeUploaded.clear();
		uploadMutex.unlock();

		glm::vec3 position = camera->GetPosition();
		glm::ivec2 fpos = { zm::Floor(position.x), zm::Floor(position.z) };
		// Check if the position crossed a chunk quadrant since last update
		glm::ivec2 newQuadrant = { fpos.x & CHUNK_HALF_WIDTH, fpos.y & CHUNK_HALF_WIDTH };
		if (quadrant != newQuadrant) {
			quadrant = newQuadrant;

			std::lock_guard<std::mutex> lk(jobMutex);
			// Additionally check if the position crossed a chunk border
			glm::ivec2 newFulcrum = { fpos.x >> CHUNK_WIDTH_BIT_DEPTH, fpos.y >> CHUNK_WIDTH_BIT_DEPTH };
			if (fulcrum != newFulcrum) {
				fulcrum = newFulcrum;
				LoadChunks();
				cv.notify_all();
			}
			else {
				UnloadChunks();
			}
		}
		
		visibleChunks.clear();
		for (const std::pair<size_t, Chunk*>& pair : chunks)
			if (camera->TestAABB(pair.second->renderPos, glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH)))
				visibleChunks.emplace_back(pair.second);
		
		//UpdateChunks();
	}

	void ChunkManager::RemeshChunk(Chunk* chunk, int xNeighbour, int zNeighbour) {
		std::lock_guard<std::mutex> lk(jobMutex);
		int jobCount = static_cast<int>(jobs.size());

		for (int x = 0; x <= zm::Abs(xNeighbour); x++) {
			for (int z = 0; z <= zm::Abs(zNeighbour); z++) {
				Chunk* neighbour = chunk->GetNeighbour(x * xNeighbour, z * zNeighbour);
				neighbour->state = Chunk::State::GENERATED;
				jobs.insert(jobs.begin(), { Task::MESH, neighbour });
			}
		}

		if (jobCount == 0)
			cv.notify_all();
	}

	//------------------------------------------------------------------------
	//	Chunk Manager Getter Functions
	//------------------------------------------------------------------------

	Chunk* ChunkManager::GetChunk(int x, int z) {
		std::unordered_map<size_t, Chunk*>::iterator iter = chunks.find(GetChunkKey(x, z));
		return (iter == chunks.end()) ? nullptr : iter->second;
	}

	Chunk* ChunkManager::GetChunk(const glm::vec3& position) {
		size_t chunkKey = GetChunkKey(zm::Floor(position.x) >> CHUNK_WIDTH_BIT_DEPTH, zm::Floor(position.z) >> CHUNK_WIDTH_BIT_DEPTH);
		std::unordered_map<size_t, Chunk*>::iterator iter = chunks.find(chunkKey);
		return (iter == chunks.end()) ? nullptr : iter->second;
	}

	glm::ivec3 ChunkManager::GetChunkCoord(const glm::vec3& position) {
		return {
			zm::Floor(position.x) >> CHUNK_WIDTH_BIT_DEPTH,
			zm::Floor(position.y) >> CHUNK_HEIGHT_BIT_DEPTH,
			zm::Floor(position.z) >> CHUNK_WIDTH_BIT_DEPTH
		};
	}

	const std::unordered_map<size_t, Chunk*>& ChunkManager::GetChunks() {
		return chunks;
	}

	const std::vector<Chunk*>& ChunkManager::GetVisibleChunks() {
		return visibleChunks;
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
			
			if (!CoordInRenderRange(chunk->chunkPos.x, chunk->chunkPos.y)) {
				CancelJobs(chunk);

				// All neighbours are ok with unload
				if (UnloadNeighbours(chunk)) {
					delete iter->second;
					iter = chunks.erase(iter);
					continue;
				}
			}
			// Chunk unload could not be completed
			++iter;
		}
	}

	size_t ChunkManager::GetChunkKey(int x, int z) {
		size_t result = static_cast<uint>(x);
		return (result << 32) | static_cast<uint>(z);
	}

	bool ChunkManager::CoordInRenderRange(int x, int z) {
		x -= fulcrum.x; z -= fulcrum.y;
		return (x * x) + (z * z) <= chunkRadiusSquared;
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
				if (neighbour->state < Chunk::State::MESHED)
					CancelJob({ Task::MESH, neighbour });
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
				// Generate the chunk
				generate(j.chunk);
				// Change the chunk state to generated
				jobMutex.lock();
				j.chunk->state = Chunk::State::GENERATED;
				jobMutex.unlock();
				break;
			case Task::MESH:
				// Mesh the chunk
				j.chunk->Mesh();
				// Change the chunk state to meshed
				jobMutex.lock();
				j.chunk->state = Chunk::State::MESHED;
				jobMutex.unlock();
				// Add the chunk to the upload list
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

		for (std::vector<Job>::iterator iter = jobs.begin(); iter != jobs.end(); iter++) {
			if (iter->task == Task::GENERATE)
				iter->chunk->state = Chunk::State::GENERATING;
			else if (iter->task == Task::MESH && iter->chunk->CanBeMeshed())
				iter->chunk->state = Chunk::State::MESHING;
			else
				continue;

			Job job = *iter;
			jobs.erase(iter);
			return job;
		}
		return { Task::NONE, nullptr };
	}

	void ChunkManager::SortJobs() {
		std::sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
			return GetDistanceToJob(a) < GetDistanceToJob(b);
		});
	}

	int ChunkManager::GetDistanceToJob(const Job& job) {
		return zm::SqrDist(job.chunk->chunkPos.x, job.chunk->chunkPos.y, fulcrum.x, fulcrum.y);
	}

	void ChunkManager::CancelJobs(Chunk* chunk) {
		for (std::vector<Job>::iterator job = jobs.begin(); job != jobs.end();)
			(job->chunk == chunk) ? job = jobs.erase(job) : ++job;
	}

	void ChunkManager::CancelJob(Job job) {
		for (std::vector<Job>::iterator j = jobs.begin(); j != jobs.end(); j++) {
			if (j->chunk == job.chunk && j->task == job.task) {
				jobs.erase(j);
				return;
			}
		}
	}
}