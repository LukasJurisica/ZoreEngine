#include "graphics/opengl/GLMesh.hpp"
#include "graphics/Buffer.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//  OpenGL Mesh Class
	//========================================================================

	GLMesh::GLMesh(void* vertices, unsigned int vsize, void* indices, unsigned int isize, const std::string& layout) :
	Mesh(isize / sizeof(Index), true), vb(new GLVertexBuffer(vertices, vsize)), ib(new GLIndexBuffer(indices, isize)) {
		// Create the Vertex Array Object
		glCreateVertexArrays(1, &meshID);
		glBindVertexArray(meshID);

		// Bind Vertex Buffer and Index buffer, and set the Vertex Buffer Layout
		vb->Bind();
		ib->Bind();
		BufferLayout::Get(layout)->Bind();

		// Unbind the Vertex Array Object, so that any following operations will not unintentionally affect it
		glBindVertexArray(0);
	}

	GLMesh::GLMesh(void* vertices, unsigned int size, const std::string& layout) :
		Mesh(size / sizeof(Index), false), vb(new GLVertexBuffer(vertices, size)), ib(nullptr) {
		// Create the Vertex Array Object
		glCreateVertexArrays(1, &meshID);
		glBindVertexArray(meshID);

		// Bind Vertex Buffer and set the Vertex Buffer Layout
		vb->Bind();
		BufferLayout::Get(layout)->Bind();

		// Unbind the Vertex Array Object, so that any following operations will not unintentionally affect it
		glBindVertexArray(0);
	}

	GLMesh::~GLMesh() {
		glDeleteVertexArrays(1, &meshID);
	}

	void GLMesh::Bind() const {
		glBindVertexArray(meshID);
	}

	void GLMesh::UnBind() const {
		glBindVertexArray(0);
	}
}