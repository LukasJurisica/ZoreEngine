#pragma once

#include "zore/utils/sized_integer.hpp"
#include <vector>
#include <string>

namespace zore {

	class Shader;

	//========================================================================
	//	Vertex Buffer Layout Element
	//========================================================================

	enum class VertexDataType { BOOL, INT_8, UINT_8, INT_16, UINT_16, INT_32, UINT_32, FLOAT, DOUBLE };

	struct VertexElement {
		VertexElement(std::string name, VertexDataType type, unsigned int count, bool normalize = false);

		std::string name;
		VertexDataType type;
		unsigned int count;
		bool normalize;
	};

	//========================================================================
	//	Vertex Buffer Layout
	//========================================================================

	class VertexLayout {
	public:
		VertexLayout();
		VertexLayout(Shader& shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements = {}, uint32_t interval = 1u);
		VertexLayout(const VertexLayout&) = delete;
		VertexLayout(VertexLayout&&) = delete;
		VertexLayout& operator=(const VertexLayout&) = delete;
		VertexLayout& operator=(VertexLayout&&) = delete;
		~VertexLayout();

		uint32_t GetID() const;
		void Set(Shader& shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements = {}, uint32_t interval = 1u);
		void Bind() const;

	private:
		void InitAttributes(const std::vector<VertexElement>& elements, uint32_t interval, uint32_t shaderID);

	private:
		uint32_t m_id;
	};
}