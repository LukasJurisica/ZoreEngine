#pragma once
#include "graphics/VertexLayout.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Vertex Buffer Layout Class
	//========================================================================

	class GLVertexLayout : public VertexLayout {
	public:
		GLVertexLayout(Shader* shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements, unsigned int interval);

		void Bind() const override;
		void Unbind() const override;

	private:
		void InitAttributes(const std::vector<VertexElement>& elements, unsigned int index, unsigned int interval);

		unsigned int id;
		unsigned int shaderID;
	};
}