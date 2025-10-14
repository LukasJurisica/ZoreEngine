#include "zore/graphics/buffer.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>

namespace zore {

	static constexpr uint32_t INVALID_BUFFER_ID = ~0;

	//========================================================================
	//	OpenGL Index Buffer
	//========================================================================

	IndexBuffer::IndexBuffer() : m_id(INVALID_BUFFER_ID) {}

	IndexBuffer::IndexBuffer(const VoidSpan& span) : m_id(0) {
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, span.SizeBytes(), span.Data(), GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(const void* data, uint32_t size) : m_id(0) {
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	uint32_t IndexBuffer::GetID() const {
		return m_id;
	}

	void IndexBuffer::Set(const VoidSpan& span) {
		Set(span.Data(), static_cast<uint32_t>(span.SizeBytes()));
	}

	void IndexBuffer::Set(const void* data, uint32_t size) {
		if (m_id == INVALID_BUFFER_ID)
			glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	void IndexBuffer::Update(const void* data, uint32_t size, uint32_t offset) {
		void* ptr = glMapNamedBufferRange(m_id, offset, size, GL_MAP_WRITE_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(m_id);
	}

	void IndexBuffer::Bind() const {
		DEBUG_ENSURE(m_id != INVALID_BUFFER_ID, "Attempted to bind an Unitialized IndexBuffer");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}
}