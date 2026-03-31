#include "zore/graphics/buffers/shader_storage_buffer.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	Uniform Buffer
	//========================================================================

	ShaderStorageBuffer::ShaderStorageBuffer() : m_bind_point(0) {}

	ShaderStorageBuffer::ShaderStorageBuffer(const void_span& span) : Base(span), m_bind_point(0) {}

	ShaderStorageBuffer::ShaderStorageBuffer(const void* data, size_t size) : Base(data, size), m_bind_point(0) {}

	ShaderStorageBuffer::ShaderStorageBuffer(const Buffer::Base& other) : Base(other), m_bind_point(0) {}

	void ShaderStorageBuffer::Bind() const {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bind_point, GetID());
	}

	void ShaderStorageBuffer::Bind(uint32_t bind_point) {
		m_bind_point = bind_point;
		Bind();
	}
}