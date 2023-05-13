#include "zore/graphics/VertexLayout.hpp"
#include "zore/graphics/Shader.hpp"
#include "zore/debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	Vertex Buffer Layout Element Struct
	//========================================================================

	VertexElement::VertexElement(std::string name, VertexDataType type, unsigned int count, bool normalize) :
		name(name), type(type), count(count), normalize(normalize) {
		ENSURE(count > 0 && count < 5, "Invalid number of components for Vertex Element");
	}

	//========================================================================
	//	Vertex Buffer Layout Class
	//========================================================================

	VertexLayout::VertexLayout() {
		glCreateVertexArrays(1, &m_id);
		glBindVertexArray(m_id);
	}

	VertexLayout::VertexLayout(Shader& shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements, uint32_t interval) : m_id(0) {
		Set(shader, vertexElements, instanceElements, interval);
	}

	VertexLayout::~VertexLayout() {
		glDeleteVertexArrays(1, &m_id);
	}

	uint32_t VertexLayout::GetID() const {
		return m_id;
	}

	void VertexLayout::Set(Shader& shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements, uint32_t interval) {
		if (m_id)
			glDeleteVertexArrays(1, &m_id);
		glCreateVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		InitAttributes(vertexElements, 0, shader.GetID());
		InitAttributes(instanceElements, interval, shader.GetID());
	}

	void VertexLayout::Bind() const {
		glBindVertexArray(m_id);
	}

	void VertexLayout::InitAttributes(const std::vector<VertexElement>& elements, uint32_t interval, uint32_t shaderID) {
		if (elements.size() == 0)
			return;

		uint32_t offset = 0;
		uint32_t index = (interval == 0) ? 0 : 1;
		for (const VertexElement& e : elements) {
			// Verify that the element exists in the shader
			int32_t loc = glGetAttribLocation(shaderID, e.name.c_str());
			if (loc == -1) {
				Logger::Warn("Invalid vertex layout buffer element name: " + e.name);
				continue;
			}

			// Get the OpenGL representation of the data type
			static const uint32_t VertexDataTypeSize[] = { 1u, 1u, 1u, 2u, 2u, 4u, 4u, 4u };
			static const uint32_t VertexDataTypeToGLDataType[] = {
				GL_BOOL, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE
			};
			uint32_t type = VertexDataTypeToGLDataType[static_cast<uint32_t>(e.type)];

			// Set the Vertex Attribute Format
			glEnableVertexArrayAttrib(m_id, loc);
			if (type == GL_DOUBLE)
				glVertexAttribLFormat(loc, e.count, type, offset);
			else if (type == GL_FLOAT || e.normalize)
				glVertexAttribFormat(loc, e.count, type, true, offset);
			else
				glVertexAttribIFormat(loc, e.count, type, offset);
			glVertexAttribBinding(loc, index);
			
			offset += VertexDataTypeSize[static_cast<uint32_t>(e.type)] * e.count;
		}
		glVertexBindingDivisor(index, interval);
	}
}