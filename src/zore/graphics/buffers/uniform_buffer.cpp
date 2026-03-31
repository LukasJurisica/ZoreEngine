#include "zore/graphics/buffers/uniform_buffer.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	Uniform Buffer
	//========================================================================

	UniformBuffer::UniformBuffer() : m_bind_point(0) {}

	UniformBuffer::UniformBuffer(const void_span& span) : Base(span), m_bind_point(0) {}

	UniformBuffer::UniformBuffer(const void* data, size_t size) : Base(data, size), m_bind_point(0) {}

	void UniformBuffer::Bind() const {
		glBindBufferBase(GL_UNIFORM_BUFFER, m_bind_point, GetID());
	}
	
	void UniformBuffer::Bind(uint32_t bind_point) {
		m_bind_point = bind_point;
		Bind();
	}
}