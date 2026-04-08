#include "zore/graphics/buffers/buffer_base.hpp"
#include "zore/structures/object_pool.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>
#include <cstring>

namespace zore::Buffer {

	struct Data {
		uint32_t id = GL_INVALID_NAME;
		uint32_t ref_count = 1;
	};

	static object_pool<Buffer::Data> s_buffer_pool;
	static bool* s_context_active = nullptr;

	//========================================================================
	//	Buffer Base
	//========================================================================

	Base::Base() : m_index(s_buffer_pool.INVALID_INDEX) {}

	Base::Base(const void_span& span) : m_index(s_buffer_pool.INVALID_INDEX) {
		Set(span);
	}

	Base::Base(const void* data, size_t size) : m_index(s_buffer_pool.INVALID_INDEX) {
		Set(data, size);
	}

	Base::Base(const Base& other) {
		Copy(other);
	}

	Base::Base(Base&& other) noexcept {
		Move(other);
	}

	Base& Base::operator=(const Base& other) {
		Copy(other);
		return *this;
	}

	Base& Base::operator=(Base&& other) noexcept {
		Move(other);
		return *this;
	}

	void Base::Move(Base& other) {
		m_index = other.m_index;
		other.m_index = s_buffer_pool.INVALID_INDEX;
	}

	void Base::Copy(const Base& other) {
		m_index = other.m_index;
		if (m_index != s_buffer_pool.INVALID_INDEX)
			s_buffer_pool[m_index].ref_count++;
	}

	Base::~Base() {
		if (s_context_active == nullptr || m_index == s_buffer_pool.INVALID_INDEX)
			return;
		if (s_buffer_pool[m_index].ref_count == 1) {
			glDeleteBuffers(1, &s_buffer_pool[m_index].id);
			s_buffer_pool[m_index].id = GL_INVALID_NAME;
			s_buffer_pool.release(m_index);
		}
		else
			s_buffer_pool[m_index].ref_count--;
	}

	void Base::Init() {
		s_context_active = new bool(true);
	}

	void Base::FreeAll() {
		delete s_context_active;
		s_context_active = nullptr;
		for (const Buffer::Data& data : s_buffer_pool)
			if (data.id != GL_INVALID_NAME)
				glDeleteBuffers(1, &data.id);
	}

	uint32_t Base::GetID() const {
		return s_buffer_pool[m_index].id;
	}

	void Base::Set(const void_span& span) {
		Set(span.data(), span.size_bytes());
	}

	void Base::Set(const void* data, size_t size) {
		DEBUG_ENSURE(s_context_active != nullptr, "Attempted to initialize a buffer without an active context.");
		if (m_index == s_buffer_pool.INVALID_INDEX) {
			m_index = s_buffer_pool.acquire(0, 1);
			glCreateBuffers(1, &s_buffer_pool[m_index].id);
		}
		glNamedBufferData(s_buffer_pool[m_index].id, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
	}

	void Base::Update(const void* data, size_t size, size_t offset) {
		DEBUG_ENSURE(s_context_active != nullptr, "Attempted to update a buffer without an active context.");
		if (m_index == s_buffer_pool.INVALID_INDEX)
			return;
		void* ptr = glMapNamedBufferRange(s_buffer_pool[m_index].id, offset, size, GL_MAP_WRITE_BIT);
		std::memcpy(ptr, data, size);
		glUnmapNamedBuffer(s_buffer_pool[m_index].id);
	}
}