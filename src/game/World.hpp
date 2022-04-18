#include "voxel/Chunk.hpp"
#include "glm/vec3.hpp"

namespace zore {

	class World {
	public:
		static bool SetBlock(ushort block, const glm::vec3& pos, const glm::vec3& ray, int range = 0);
		static bool RaycastBlock(const glm::vec3& pos, const glm::vec3& ray, glm::ivec3& out, int range = 0);
		static uint RaycastBlockCountFaces(const glm::vec3& pos, const glm::vec3& ray, glm::ivec3& pout, glm::ivec3& fout, int range = 0);
		static glm::bvec3 DoCollision(const glm::vec3& pos, glm::vec3& velocity, const glm::vec3& size);

	private:
		static bool CheckCollisionOnAxis(int axis, int blockOffset, Chunk* chunk, const glm::vec3& local, const glm::vec3& aabb);
		static Chunk* RaycastBlock(glm::vec3 pos, glm::vec3 ray, glm::ivec3& out, int& axis, int range = 0);
	};
}