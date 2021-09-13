#pragma once
#include <vector>
#include "graphics/Shader.hpp"
#include "graphics/Buffer.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Vertex Buffer Layout Class
	//========================================================================

	enum class VertexDataType { BOOL, BYTE, UBYTE, INT, FLOAT };

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
		static VertexLayout* Get(const std::string& name);
		virtual ~VertexLayout();

		virtual void SetInstanceArrayLayout(const std::vector<VertexElement>& elements, unsigned int interval = 1u) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	protected:
		VertexLayout(const std::string& name);

		std::string name;
		static const unsigned int VertexDataTypeSize[5];
	};
}