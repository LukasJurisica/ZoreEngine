#pragma once
#include "graphics/Shader.hpp"
#include <vector>

namespace zore {

	//========================================================================
	//	Platform Agnostic Vertex Buffer Layout Class
	//========================================================================

	enum class VertexDataType { BOOL, BYTE, UBYTE, INT, UINT, FLOAT, DOUBLE };

	struct VertexElement {
		VertexElement(std::string name, VertexDataType type, unsigned int count, bool normalize = false);

		std::string name;
		VertexDataType type;
		unsigned int count;
		bool normalize;
	};

	class VertexLayout {
	public:
		static VertexLayout* Create(Shader* shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements = {}, unsigned int interval = 1u);
		virtual ~VertexLayout() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	protected:
		static const unsigned int VertexDataTypeSize[6];
	};
}