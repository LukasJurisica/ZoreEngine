#include "graphics/Mesh.hpp"
#include "graphics/opengl/GLMesh.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"


namespace zore {

	//========================================================================
	//  Platform Agnostic Mesh Class
	//========================================================================

	Mesh::Mesh(unsigned int count, bool indexed) : count(count), indexed(indexed) {}

	Mesh* Mesh::Create(void* vertices, unsigned int vsize, void* indices, unsigned int isize, const std::string& layout) {
		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			return new GLMesh(vertices, vsize, indices, isize, layout);
		}
		throw ZORE_EXCEPTION("Invalid active RenderAPI");
		return nullptr;
	}

	Mesh* Mesh::Create(void* vertices, unsigned int size, const std::string& layout) {
		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			return new GLMesh(vertices, size, layout);
		}
		throw ZORE_EXCEPTION("Invalid active RenderAPI");
		return nullptr;
	}

	unsigned int Mesh::GetCount() const {
		return count;
	}

	bool Mesh::IsIndexed() const {
		return indexed;
	}
}