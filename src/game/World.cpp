#include "game/World.hpp"
#include "voxel/ChunkManager.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

namespace zore {

	bool World::PlaceBlock(glm::vec3 pos, const glm::vec3& ray, ushort block, int range) {
		glm::ivec3 out;
		int axis;
		Chunk* chunk = RaycastBlock(pos, ray, out, axis, range);
		if (!chunk)
			return false;
		out[axis] -= zm::Sign(ray[axis]);

		glm::ivec2 chunkOffset = { out.x >> Chunk::CHUNK_WIDTH_BIT_DEPTH, out.z >> Chunk::CHUNK_WIDTH_BIT_DEPTH, };
		chunk = chunk->GetNeighbour(chunkOffset.x, chunkOffset.y);
		out.x -= chunkOffset.x * Chunk::CHUNK_WIDTH;
		out.z -= chunkOffset.y * Chunk::CHUNK_WIDTH;
		chunk->SetBlock(out.x, out.y, out.z, block);
		ChunkManager::RemeshChunk(chunk,
			out.x == 0 ? -1 : (out.x == Chunk::CHUNK_WIDTH - 1 ? 1 : 0),
			out.z == 0 ? -1 : (out.z == Chunk::CHUNK_WIDTH - 1 ? 1 : 0)
		);
		
		return true;
	}

	bool World::BreakBlock(glm::vec3 pos, const glm::vec3& ray, int range) {
		glm::ivec3 out;
		int axis;
		Chunk* chunk = RaycastBlock(pos, ray, out, axis, range);
		if (!chunk)
			return false;
		
		glm::ivec2 chunkOffset = { out.x >> Chunk::CHUNK_WIDTH_BIT_DEPTH, out.z >> Chunk::CHUNK_WIDTH_BIT_DEPTH, };
		chunk = chunk->GetNeighbour(chunkOffset.x, chunkOffset.y);
		out.x -= chunkOffset.x * Chunk::CHUNK_WIDTH;
		out.z -= chunkOffset.y * Chunk::CHUNK_WIDTH;
		chunk->SetBlock(out.x, out.y, out.z, BLOCK_AIR);
		ChunkManager::RemeshChunk(chunk,
			out.x == 0 ? -1 : (out.x == Chunk::CHUNK_WIDTH - 1 ? 1 : 0),
			out.z == 0 ? -1 : (out.z == Chunk::CHUNK_WIDTH - 1 ? 1 : 0)
		);

		return true;
	}

	bool World::RaycastBlock(glm::vec3 pos, const glm::vec3& ray, glm::ivec3& out, int range) {
		int axis;
		Chunk* chunk = RaycastBlock(pos, ray, out, axis, range);
		if (!chunk)
			return false;
		out += chunk->GetPosition();
		return true;
	}

	Chunk* World::RaycastBlock(glm::vec3 pos, glm::vec3 ray, glm::ivec3& out, int& axis, int range) {
		Chunk* chunk = ChunkManager::GetChunk(pos);
		pos -= chunk->GetPosition();
		DEBUG_ENSURE(range <= Chunk::CHUNK_HALF_WIDTH, "The maximum block raycast range is half a chunk width");
		range *= range;
		out = { zm::Floor(pos.x), zm::Floor(pos.y), zm::Floor(pos.z) };
		ray = 1.f / ray;

		glm::ivec3 offset = { 0, 0, 0 };
		glm::ivec3 step = { zm::Sign(ray.x), zm::Sign(ray.y) , zm::Sign(ray.z) };
		glm::vec3 delta = { step.x * ray.x, step.y * ray.y, step.z * ray.z };
		glm::vec3 weight = {
			(out.x + ((step.x + 1) >> 1) - pos.x) * ray.x,
			(out.y + ((step.y + 1) >> 1) - pos.y) * ray.y,
			(out.z + ((step.z + 1) >> 1) - pos.z) * ray.z
		};

		do {
			axis = (weight.x < weight.y && weight.x < weight.z) ? 0 : (weight.y < weight.z ? 1 : 2);
			offset[axis]++;
			if ((offset.x * offset.x) + (offset.y * offset.y) + (offset.z * offset.z) > range)
				return nullptr;
			out[axis] += step[axis];
			//if (blockPos[minIndex] == out[minIndex]) { "OUT OF BOUNDS" }
			weight[axis] += delta[axis];
		} while (chunk->GetBlock(out.x, out.y, out.z) == BLOCK_AIR);

		return chunk;
	}

	glm::bvec3 World::DoCollision(const glm::vec3& pos, glm::vec3& velocity, const glm::vec3& size) {
		glm::vec3 target = pos + velocity;
		Chunk* chunk = ChunkManager::GetChunk(target);
		glm::vec3 chunkPos = chunk->GetPosition();
		glm::vec3 localPosition = pos - chunkPos;

		glm::vec3 offset = { 0.f, 0.f, 0.f };
		for (int i = 0; i < 3; i++)
			offset[i] = (velocity[i] > 0) ? size[i] : -size[i];
		glm::ivec3 blockOffset = glm::floor(localPosition + velocity + offset);

		glm::bvec3 result = { false, false, false };
		for (int i = 0; i < 3; i++) {
			if (CheckCollisionOnAxis(i, blockOffset[i], chunk, localPosition, size)) {
				velocity[i] = 0;
				result[i] = true;
			}

			//	result[i] = CheckCollisionOnAxis(i, blockOffset[i], chunk, localPosition, aabb);
			//	velocity[i] *= result[i];
		}
		return result;
	}

	bool World::CheckCollisionOnAxis(int axis, int blockOffset, Chunk* chunk, const glm::vec3& local, const glm::vec3& size) {
		int a1 = (axis + 1) % 3;
		int a2 = (axis + 2) % 3;

		for (int a = zm::Floor(local[a1] - size[a1]); a <= zm::Floor(local[a1] + size[a1]); a++) {
			for (int b = zm::Floor(local[a2] - size[a2]); b <= zm::Floor(local[a2] + size[a2]); b++) {

				glm::ivec3 search;
				search[axis] = blockOffset;
				search[a1] = a;
				search[a2] = b;

				if (chunk->GetBlock(search.x, search.y, search.z) & BLOCK_BIT)
					return true;
			}
		}

		return false;
	}
}