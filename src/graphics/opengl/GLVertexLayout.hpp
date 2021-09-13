#pragma once
#include "graphics/VertexLayout.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Vertex Buffer Layout Class
	//========================================================================

	class GLVertexLayout : public VertexLayout {
	public:
		GLVertexLayout(const std::string& name, Shader* shader, const std::vector<VertexElement>& elements);

		void SetInstanceArrayLayout(const std::vector<VertexElement>& elements, unsigned int interval) override;
		void Bind() const override;
		void Unbind() const override;

	private:
		void InitAttributes(const std::vector<VertexElement>& elements, unsigned int index, unsigned int interval);

		unsigned int id;
		unsigned int shaderID;
		static const unsigned int VertexDataTypeToGLDataType[5];
	};
}