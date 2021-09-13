#include "graphics/opengl/GLBuffer.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Vertex Buffer Class
	//========================================================================

	GLVertexBuffer::GLVertexBuffer(void* data, unsigned int size, unsigned int stride) : stride(stride) {
		glCreateBuffers(1, &id);
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	GLVertexBuffer::~GLVertexBuffer() {
		glDeleteBuffers(1, &id);
	}

	void GLVertexBuffer::Set(const void* data, unsigned int size, unsigned int stride) {
		if (stride > 0)
			this->stride = stride;
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	void GLVertexBuffer::Update(const void* data, unsigned int size, unsigned int offset) {
		void* ptr = glMapNamedBufferRange(id, offset, size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(id);
	}

	void GLVertexBuffer::Bind() const {
		glBindVertexBuffer(0, id, 0, stride);
	}

	void GLVertexBuffer::Unbind() const {
		glBindVertexBuffer(0, 0, 0, 0);
	}

	//========================================================================
	//	OpenGL Index Buffer Class
	//========================================================================

	GLIndexBuffer::GLIndexBuffer(void* data, unsigned int size) {
		glCreateBuffers(1, &id);
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer() {
		glDeleteBuffers(1, &id);
	}

	void GLIndexBuffer::Set(const void* data, unsigned int size) {
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	void GLIndexBuffer::Update(const void* data, unsigned int size, unsigned int offset) {
		void* ptr = glMapNamedBufferRange(id, offset, size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(id);
	}

	void GLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void GLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	//========================================================================
	//	OpenGL Instance Buffer Class
	//========================================================================

	GLInstanceArrayBuffer::GLInstanceArrayBuffer(void* data, unsigned int size, unsigned int stride) : stride(stride) {
		glCreateBuffers(1, &id);
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	GLInstanceArrayBuffer::~GLInstanceArrayBuffer() {
		glDeleteBuffers(1, &id);
	}

	void GLInstanceArrayBuffer::Set(const void* data, unsigned int size, unsigned int stride) {
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	void GLInstanceArrayBuffer::Update(const void* data, unsigned int size, unsigned int offset) {
		void* ptr = glMapNamedBufferRange(id, offset, size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(id);
	}

	void GLInstanceArrayBuffer::Bind() const {
		glBindVertexBuffer(1, id, 0, stride);
	}

	void GLInstanceArrayBuffer::Unbind() const {
		glBindVertexBuffer(1, id, 0, stride);
	}

	//========================================================================
	//	OpenGL Shader Storage Buffer Class
	//========================================================================

	GLShaderStorageBuffer::GLShaderStorageBuffer(void* data, unsigned int size, unsigned int index) : index(index) {
		glCreateBuffers(1, &id);
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	GLShaderStorageBuffer::~GLShaderStorageBuffer() {
		glDeleteBuffers(1, &id);
	}

	void GLShaderStorageBuffer::Set(const void* data, unsigned int size) {
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	void GLShaderStorageBuffer::Update(const void* data, unsigned int size, unsigned int offset) {
		void* ptr = glMapNamedBufferRange(id, offset, size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(id);
	}

	void GLShaderStorageBuffer::Bind() const {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id);
	}

	void GLShaderStorageBuffer::Unbind() const {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, 0);
	}

	//========================================================================
	//	OpenGL Uniform Buffer Class
	//========================================================================

	GLUniformBuffer::GLUniformBuffer(void* data, unsigned int size, unsigned int index) : index(index) {
		glCreateBuffers(1, &id);
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	GLUniformBuffer::~GLUniformBuffer() {
		glDeleteBuffers(1, &id);
	}

	void GLUniformBuffer::Set(const void* data, unsigned int size) {
		glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	}

	void GLUniformBuffer::Update(const void* data, unsigned int size, unsigned int offset) {
		void* ptr = glMapNamedBufferRange(id, offset, size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(id);
	}

	void GLUniformBuffer::Bind() const {
		glBindBufferBase(GL_UNIFORM_BUFFER, index, id);
	}

	void GLUniformBuffer::Unbind() const {
		glBindBufferBase(GL_UNIFORM_BUFFER, index, 0);
	}
}