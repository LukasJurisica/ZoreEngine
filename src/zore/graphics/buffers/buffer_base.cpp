#include "zore/graphics/buffers/buffer_base.hpp"
#include "zore/graphics/render_engine.hpp"
#include "zore/graphics/gl_handle.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>
#include <cstring>

namespace zore::Buffer {

	static GLHandleRegistry s_registry(Base::Delete);

	//========================================================================
	//	Buffer Base
	//========================================================================

	Base::Base() : m_index(s_registry->INVALID_INDEX) {}

	Base::Base(const void_span& span) : m_index(s_registry->INVALID_INDEX) {
		Set(span);
	}

	Base::Base(const void* data, size_t size) : m_index(s_registry->INVALID_INDEX) {
		Set(data, size);
	}

	Base::Base(Base&& other) noexcept {
		Move(other);
	}

	Base& Base::operator=(Base&& other) noexcept {
		Move(other);
		return *this;
	}

	void Base::Move(Base& other) {
		m_index = other.m_index;
		other.m_index = s_registry.m_pool.INVALID_INDEX;
	}

	Base::~Base() {
		if (RenderEngine::Active() && m_index != s_registry->INVALID_INDEX) {
			Delete(s_registry[m_index]);
			s_registry->release(m_index);
		}
	}

	uint32_t Base::GetID() const {
		return s_registry[m_index];
	}

	void Base::Set(const void_span& span) {
		Set(span.data(), span.size_bytes());
	}

	void Base::Set(const void* data, size_t size) {
		DEBUG_ENSURE(RenderEngine::Active(), "Attempted to initialize a buffer without an active context.");
		if (m_index == s_registry->INVALID_INDEX) {
			m_index = s_registry->acquire(0);
			glCreateBuffers(1, &s_registry[m_index]);
		}
		glNamedBufferData(s_registry[m_index], static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
	}

	void Base::Update(const void* data, size_t size, size_t offset) {
		DEBUG_ENSURE(RenderEngine::Active(), "Attempted to update a buffer without an active context.");
		if (m_index == s_registry->INVALID_INDEX)
			return;
		void* ptr = glMapNamedBufferRange(s_registry[m_index], offset, size, GL_MAP_WRITE_BIT);
		std::memcpy(ptr, data, size);
		glUnmapNamedBuffer(s_registry[m_index]);
	}

	void Base::Delete(uint32_t& id) {
		if (id != GL_INVALID_NAME) {
			glDeleteBuffers(1, &id);
			id = GL_INVALID_NAME;
		}
	}
}