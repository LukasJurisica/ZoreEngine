#pragma once
#include "graphics/Mesh.hpp"
#include "graphics/opengl/GLBuffer.hpp"

namespace zore {

	//========================================================================
	//  OpenGL Mesh Class
	//========================================================================

	class GLMesh : public Mesh {
	public:
		GLMesh(void* vertices, unsigned int vsize, void* indices, unsigned int isize, const std::string& layout);
		GLMesh(void* vertices, unsigned int size, const std::string& layout);
		~GLMesh();

		void Bind() const override;
		void UnBind() const override;

	private:
		unsigned int meshID;
		GLVertexBuffer* vb;
		GLIndexBuffer* ib;
	};
}