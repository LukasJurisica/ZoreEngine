#include "zore/graphics/vertex_layout.hpp"
#include "zore/graphics/shader.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	Vertex Buffer Layout Element
	//========================================================================

	VertexElement::VertexElement(std::string name, Type type, unsigned int count, bool normalize) :
		name(name), type(type), count(count), normalize(normalize) {
		ENSURE(count > 0 && count < 5, "Invalid number of components for Vertex Element");
	}

	//========================================================================
	//	Vertex Buffer Layout
	//========================================================================

	VertexLayout::VertexLayout() : m_id(GL_INVALID_NAME) {}

	VertexLayout::VertexLayout(Shader& shader, const std::vector<VertexElement>& vertex_elements, const std::vector<VertexElement>& instance_elements, uint32_t interval) : m_id(GL_INVALID_NAME) {
		Set(shader, vertex_elements, instance_elements, interval);
	}

	VertexLayout::~VertexLayout() {
		Free();
	}

	void VertexLayout::Init() {
		if (m_id == GL_INVALID_NAME)
			glCreateVertexArrays(1, &m_id);
		glBindVertexArray(m_id);
	}

	void VertexLayout::Free() {
		if (m_id != GL_INVALID_NAME) {
			glDeleteVertexArrays(1, &m_id);
			m_id = GL_INVALID_NAME;
		}
	}

	uint32_t VertexLayout::GetID() const {
		return m_id;
	}

	void VertexLayout::Set(Shader& shader, const std::vector<VertexElement>& vertex_elements, const std::vector<VertexElement>& instance_elements, uint32_t interval) {
		if (m_id != GL_INVALID_NAME)
			glDeleteVertexArrays(1, &m_id);
		glCreateVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		InitAttributes(vertex_elements, 0, shader.GetID());
		InitAttributes(instance_elements, interval, shader.GetID());
	}

	void VertexLayout::Bind() const {
		if (m_id != GL_INVALID_NAME)
			glBindVertexArray(m_id);
	}

	void VertexLayout::Unbind() {
		Empty().Bind();
	}

	VertexLayout& VertexLayout::Empty() {
		static VertexLayout empty;
		return empty;
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
			static const uint32_t s_vertex_data_type_size[] = { 1u, 1u, 1u, 2u, 2u, 4u, 4u, 4u };
			static const uint32_t s_vertex_data_type_to_gl_data_type[] = {
				GL_BOOL, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE
			};
			uint32_t type = s_vertex_data_type_to_gl_data_type[static_cast<uint32_t>(e.type)];

			// Set the Vertex Attribute Format
			glEnableVertexArrayAttrib(m_id, loc);
			if (type == GL_DOUBLE)
				glVertexAttribLFormat(loc, e.count, type, offset);
			else if (type == GL_FLOAT || e.normalize)
				glVertexAttribFormat(loc, e.count, type, true, offset);
			else
				glVertexAttribIFormat(loc, e.count, type, offset);
			glVertexAttribBinding(loc, index);
			
			offset += s_vertex_data_type_size[static_cast<uint32_t>(e.type)] * e.count;
		}
		glVertexArrayBindingDivisor(m_id, index, interval);
	}
}