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

	Mesh* Mesh::Create(void* vertices, unsigned int size, unsigned int stride) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new Mesh(size / stride, new GLVertexBuffer(vertices, size, stride), nullptr);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	Mesh* Mesh::Create(void* vertices, unsigned int vsize, unsigned int stride, void* indices, unsigned int isize) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new Mesh(isize / sizeof(Index), new GLVertexBuffer(vertices, vsize, stride), new GLIndexBuffer(indices, isize));
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

	void Mesh::SetCount(unsigned int count) {
		this->count = count;
	}

	unsigned int Mesh::GetCount() const {
		return count;
	}

	bool Mesh::IsIndexed() const {
		return ib;
	}
}