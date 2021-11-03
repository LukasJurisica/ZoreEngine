#pragma once
#include "graphics/Shader.hpp"
#include <vector>

namespace zore {

	//========================================================================
	//	Platform Agnostic Vertex Buffer Layout Class
	//========================================================================

	enum class VertexDataType { BOOL, BYTE, UBYTE, INT, UINT, FLOAT };

	struct VertexElement {
		VertexElement(std::string name, VertexDataType type, unsigned int count, bool normalize = false);

		std::string name;
		VertexDataType type;
		unsigned int count;
		bool normalize;
	};

	class VertexLayout {
	public:
		static VertexLayout* Create(const std::string& name, Shader* shader, const std::vector<VertexElement>& elements);
		static VertexLayout* Create(const std::string& name, Shader* shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements, unsigned int interval = 1u);
		static VertexLayout* Get(const std::string& name);
		virtual ~VertexLayout();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	protected:
		VertexLayout(const std::string& name);

		std::string name;
		static const unsigned int VertexDataTypeSize[6];
	};
}