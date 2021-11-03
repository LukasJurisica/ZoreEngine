#include "graphics/Mesh.hpp"
#include "graphics/opengl/GLBuffer.hpp"
#include "graphics/VertexLayout.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//  Platform Agnostic Mesh Class
	//========================================================================

	Mesh::Mesh(unsigned int count, VertexBuffer* vb, IndexBuffer* ib) : count(count), vb(vb), ib(ib) {}

	Mesh::~Mesh() {
		delete vb;
		delete ib;
	}

	Mesh* Mesh::Create(const void* vertices, unsigned int stride, unsigned int count) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new Mesh(count, new GLVertexBuffer(vertices, count * stride, stride), nullptr);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	Mesh* Mesh::Create(const void* vertices, unsigned int stride, unsigned int vCount, const void* indices, unsigned int iCount) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new Mesh(iCount, new GLVertexBuffer(vertices, vCount * stride, stride), new GLIndexBuffer(indices, iCount * sizeof(Index)));
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	void Mesh::Bind() const {
		vb->Bind();
		if (ib) ib->Bind();
	}

	void Mesh::Unbind() const {
		vb->Unbind();
		if (ib) ib->Unbind();
	}

	unsigned int Mesh::GetCount() const {
		return count;
	}

	bool Mesh::IsIndexed() const {
		return ib;
	}
}