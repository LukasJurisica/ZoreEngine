#include "zore/graphics/Buffer.hpp"
#include "zore/debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Vertex Buffer Class
	//========================================================================

	VertexBuffer::VertexBuffer() : m_id(0), m_stride(0), m_index(0) {}

	VertexBuffer::VertexBuffer(const void* data, uint32_t size, uint32_t stride, bool instanced) : m_stride(stride), m_index(instanced ? 1 : 0) {
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	uint32_t VertexBuffer::GetID() const {
		return m_id;
	}

	void VertexBuffer::Set(const void* data, uint32_t size, uint32_t stride) {
		if (stride > 0)
			m_stride = stride;
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::Update(const void* data, uint32_t size, uint32_t offset) {
		void* ptr = glMapNamedBufferRange(m_id, offset, size, GL_MAP_WRITE_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(m_id);
	}

	void VertexBuffer::Bind() const {
		glBindVertexBuffer(m_index, m_id, 0, m_stride);
	}

	void VertexBuffer::Bind(bool instanced) {
		m_index = instanced ? 1 : 0;
		glBindVertexBuffer(m_index, m_id, 0, m_stride);
	}

	//========================================================================
	//	OpenGL Index Buffer Class
	//========================================================================

	IndexBuffer::IndexBuffer() {
		glCreateBuffers(1, &m_id);
	}

	IndexBuffer::IndexBuffer(const void* data, uint32_t size) {
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	uint32_t IndexBuffer::GetID() const {
		return m_id;
	}

	void IndexBuffer::Set(const void* data, uint32_t size) {
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	void IndexBuffer::Update(const void* data, uint32_t size, uint32_t offset) {
		void* ptr = glMapNamedBufferRange(m_id, offset, size, GL_MAP_WRITE_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(m_id);
	}

	void IndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	//========================================================================
	//	OpenGL Shader Storage Buffer Class
	//========================================================================

	ShaderStorageBuffer::ShaderStorageBuffer() : m_index(0) {
		glCreateBuffers(1, &m_id);
	}

	ShaderStorageBuffer::ShaderStorageBuffer(const void* data, uint32_t size) : m_index(0) {
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	ShaderStorageBuffer::~ShaderStorageBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	uint32_t ShaderStorageBuffer::GetID() const {
		return m_id;
	}

	void ShaderStorageBuffer::Set(const void* data, uint32_t size) {
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	void ShaderStorageBuffer::Update(const void* data, uint32_t size, uint32_t offset) {
		void* ptr = glMapNamedBufferRange(m_id, offset, size, GL_MAP_WRITE_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(m_id);
	}

	void ShaderStorageBuffer::Bind() const {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, m_id);
	}

	void ShaderStorageBuffer::Bind(uint32_t index) {
		m_index = index;
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, m_id);
	}

	//========================================================================
	//	OpenGL Uniform Buffer Class
	//========================================================================

	UniformBuffer::UniformBuffer() : m_index(0) {
		glCreateBuffers(1, &m_id);
	}

	UniformBuffer::UniformBuffer(const void* data, uint32_t size) : m_index(0) {
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	UniformBuffer::~UniformBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	uint32_t UniformBuffer::GetID() const {
		return m_id;
	}

	void UniformBuffer::Set(const void* data, uint32_t size) {
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	void UniformBuffer::Update(const void* data, uint32_t size, uint32_t offset) {
		void* ptr = glMapNamedBufferRange(m_id, offset, size, GL_MAP_WRITE_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(m_id);
	}

	void UniformBuffer::Bind() const {
		glBindBufferBase(GL_UNIFORM_BUFFER, m_index, m_id);
	}

	void UniformBuffer::Bind(uint32_t index) {
		m_index = index;
		glBindBufferBase(GL_UNIFORM_BUFFER, m_index, m_id);
	}
}