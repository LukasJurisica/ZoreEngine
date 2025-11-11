#include "zore/graphics/buffer.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>

namespace zore {

	static constexpr uint32_t INVALID_BUFFER_ID = ~0;

	//========================================================================
	//	OpenGL Uniform Buffer
	//========================================================================

	UniformBuffer::UniformBuffer() : m_id(INVALID_BUFFER_ID), m_index(0) {}

	UniformBuffer::UniformBuffer(const VoidSpan& span) : m_id(0), m_index(0) {
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, span.SizeBytes(), span.Data(), GL_STATIC_DRAW);
	}

	UniformBuffer::UniformBuffer(const void* data, uint32_t size) : m_index(0), m_id(0) {
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	UniformBuffer::~UniformBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	uint32_t UniformBuffer::GetID() const {
		return m_id;
	}

	void UniformBuffer::Set(const VoidSpan& span) {
		Set(span.Data(), static_cast<uint32_t>(span.SizeBytes()));
	}

	void UniformBuffer::Set(const void* data, uint32_t size) {
		if (m_id == INVALID_BUFFER_ID)
			glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	void UniformBuffer::Update(const void* data, uint32_t size, uint32_t offset) {
		void* ptr = glMapNamedBufferRange(m_id, offset, size, GL_MAP_WRITE_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(m_id);
	}

	void UniformBuffer::Bind() const {
		DEBUG_ENSURE(m_id != INVALID_BUFFER_ID, "Attempted to bind an Unitialized UniformBuffer");
		glBindBufferBase(GL_UNIFORM_BUFFER, m_index, m_id);
	}

	void UniformBuffer::Bind(uint32_t index) {
		m_index = index;
		Bind();
	}
}