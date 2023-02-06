#include "voxel/World.hpp"
#include "voxel/ChunkManager.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

namespace zore {

	bool World::SetBlock(ushort block, const glm::vec3& pos, const glm::vec3& ray, int range) {
		glm::ivec3 out;
		int axis;
		Chunk* chunk = RaycastBlock(pos, ray, out, axis, range);
		if (!chunk)
			return false;
		if (block != BLOCK_AIR)
			out[axis] -= zm::Sign(ray[axis]);

		glm::ivec2 chunkOffset = { out.x >> CHUNK_WIDTH_BIT_DEPTH, out.z >> CHUNK_WIDTH_BIT_DEPTH, };
		chunk = chunk->GetNeighbour(chunkOffset.x, chunkOffset.y);
		out.x -= chunkOffset.x * CHUNK_WIDTH;
		out.z -= chunkOffset.y * CHUNK_WIDTH;
		chunk->SetBlock(out.x, out.y, out.z, block);
		ChunkManager::RemeshChunk(chunk,
			out.x == 0 ? -1 : (out.x == CHUNK_WIDTH - 1 ? 1 : 0),
			out.z == 0 ? -1 : (out.z == CHUNK_WIDTH - 1 ? 1 : 0)
		);

		return true;
	}

	bool World::RaycastBlock(const glm::vec3& pos, const glm::vec3& ray, glm::ivec3& out, int range) {
		int axis;
		Chunk* chunk = RaycastBlock(pos, ray, out, axis, range);
		if (!chunk)
			return false;

		out += chunk->GetPosition();
		return true;
	}

	uint World::RaycastBlockCountFaces(const glm::vec3& pos, const glm::vec3& ray, glm::ivec3& pout, glm::ivec3& fout, int range) {
		int axis;
		Chunk* chunk = RaycastBlock(pos, ray, pout, axis, range);
		if (!chunk)
			return 0;
		
		uint count = 0;

		glm::ivec3 chunkPos = chunk->GetPosition();
		for (int i = 0; i < 3; i++) {
			glm::ivec3 p = pout;
			pout[i] += chunkPos[i];
			p[i] -= zm::Sign(ray[i]);
			if (!(chunk->GetBlock(p.x, p.y, p.z) & BLOCK_BIT) && zm::Floor(pos[i]) != pout[i]) { // Only include visible faces
				fout[count] = (i * 2) + (ray[i] < 0 ? 1 : 0);
				count += 1;
			}
		}

		return count;
	}

	Chunk* World::RaycastBlock(glm::vec3 pos, glm::vec3 ray, glm::ivec3& out, int& axis, int range) {
		Chunk* chunk = ChunkManager::GetChunk(pos);
		pos -= chunk->GetPosition();
		DEBUG_ENSURE(range <= CHUNK_HALF_WIDTH, "The maximum block raycast range is half a chunk width");
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

		bool invalidSelection = false;
		do {
			axis = (weight.x < weight.y && weight.x < weight.z) ? 0 : (weight.y < weight.z ? 1 : 2);
			offset[axis]++;
			if ((offset.x * offset.x) + (offset.y * offset.y) + (offset.z * offset.z) > range)
				return nullptr;
			out[axis] += step[axis];
			//if (blockPos[minIndex] == out[minIndex]) { "OUT OF BOUNDS" }
			weight[axis] += delta[axis];
			ushort block = chunk->GetBlock(out.x, out.y, out.z);
			invalidSelection = block == BLOCK_AIR || (!(block & BLOCK_BIT) && (block & FLUID_BIT));
		} while (invalidSelection);

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