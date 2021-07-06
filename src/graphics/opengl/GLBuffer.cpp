#include "graphics/opengl/GLBuffer.hpp"
#include "graphics/opengl/GLShader.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Base Buffer Class
	//========================================================================

	GLBuffer::GLBuffer(unsigned int type) : type(type) {
		glCreateBuffers(1, &bufferID);
	}

	GLBuffer::GLBuffer(unsigned int type, void* data, unsigned int size, unsigned int mode) : type(type) {
		glCreateBuffers(1, &bufferID);
		glNamedBufferData(bufferID, size, data, mode);
	}

	GLBuffer::~GLBuffer() {
		glDeleteBuffers(1, &bufferID);
	}

	void GLBuffer::Bind() const {
		glBindBuffer(type, bufferID);
	}

	void GLBuffer::UnBind() const {
		glBindBuffer(type, 0);
	}

	void GLBuffer::Set(const void* data, unsigned int size) {
		glNamedBufferData(bufferID, size, data, GL_DYNAMIC_DRAW);
	}

	void GLBuffer::Update(const void* data, unsigned int size, unsigned int offset) {
		void* ptr = glMapNamedBufferRange(bufferID, offset, size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		memcpy(ptr, data, size);
		glUnmapNamedBuffer(bufferID);
	}

	unsigned int GLBuffer::GetBufferID() const {
		return bufferID;
	}

	//========================================================================
	//	OpenGL Base Block Buffer Class
	//========================================================================

	GLBlockBuffer::GLBlockBuffer(unsigned int type, unsigned int index) : GLBuffer(GL_SHADER_STORAGE_BUFFER) {
		this->index = index;
	}

	GLBlockBuffer::GLBlockBuffer(unsigned int type, void* data, unsigned int size, unsigned int mode, unsigned int index) : GLBuffer(GL_SHADER_STORAGE_BUFFER) {
		this->index = index;
	}

	void GLBlockBuffer::Bind() const {
		glBindBufferBase(type, index, bufferID);
	}

	void GLBlockBuffer::UnBind() const {
		glBindBufferBase(type, index, 0);
	}

	//========================================================================
	//	OpenGL Vertex Buffer Class
	//========================================================================

	GLVertexBuffer::GLVertexBuffer() : GLBuffer(GL_ARRAY_BUFFER) {};

	GLVertexBuffer::GLVertexBuffer(void* vertices, unsigned int size) :
		GLBuffer(GL_ARRAY_BUFFER, vertices, size, GL_STATIC_DRAW) {}

	//========================================================================
	//	OpenGL Index Buffer Class
	//========================================================================

	GLIndexBuffer::GLIndexBuffer() : GLBuffer(GL_ELEMENT_ARRAY_BUFFER) {};

	GLIndexBuffer::GLIndexBuffer(void* indices, unsigned int size) :
		GLBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, size, GL_STATIC_DRAW) {
	}

	//========================================================================
	//	OpenGL Shader Storage Buffer Class
	//========================================================================

	GLShaderStorageBuffer::GLShaderStorageBuffer(unsigned int index) : GLBlockBuffer(GL_SHADER_STORAGE_BUFFER, index) {}

	GLShaderStorageBuffer::GLShaderStorageBuffer(void* data, unsigned int size, unsigned int index) :
		GLBlockBuffer(GL_SHADER_STORAGE_BUFFER, data, size, GL_STATIC_DRAW, index) {}

	//========================================================================
	//	OpenGL Uniform Buffer Class
	//========================================================================

	GLUniformBuffer::GLUniformBuffer(unsigned int index) : GLBlockBuffer(GL_UNIFORM_BUFFER, index) {}

	GLUniformBuffer::GLUniformBuffer(void* data, unsigned int size, unsigned int index) :
		GLBlockBuffer(GL_UNIFORM_BUFFER, data, size, GL_DYNAMIC_DRAW, index) {}

	void GLUniformBuffer::AttachToShader(Shader* shader, const std::string& name) {
		unsigned int programID = reinterpret_cast<GLShader*>(shader)->GetShaderID();
		unsigned int uniformBlockIndex = glGetUniformBlockIndex(programID, name.c_str());
		glUniformBlockBinding(programID, uniformBlockIndex, index);
	}

	//========================================================================
	//	OpenGL Vertex Buffer Layout Class
	//========================================================================

	GLBufferLayout::GLBufferLayout(const std::string& name, Shader* shader, const std::vector<BufferElement>& elements) : BufferLayout(name, elements) {
		for (BufferElement& e : this->elements) {
			e.attribLocation = glGetAttribLocation(reinterpret_cast<GLShader*>(shader)->GetShaderID(), e.name.c_str());
			if (e.attribLocation == -1)
				Logger::Warn("Invalid Buffer Element name \"" + e.name + "\" For layout: " + name);
			e.offset = stride;
			stride += e.size;
		}
	}

	void GLBufferLayout::Bind() const {
		for (const BufferElement& e : elements) {
			glVertexAttribPointer(e.attribLocation, e.count, SDTtoGLDT(e.type), e.normalized, stride, reinterpret_cast<void*>(e.offset));
			glEnableVertexAttribArray(e.attribLocation);
		}
	}
}