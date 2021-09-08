#include "graphics/opengl/GLBuffer.hpp"
#include "graphics/opengl/GLShader.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Vertex Buffer Class
	//========================================================================

	GLVertexBuffer::GLVertexBuffer() : stride(0) {
		glCreateBuffers(1, &id);
	}

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

	GLIndexBuffer::GLIndexBuffer() {
		glCreateBuffers(1, &id);
	}

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
	//	OpenGL Shader Storage Buffer Class
	//========================================================================

	GLShaderStorageBuffer::GLShaderStorageBuffer(unsigned int index) : index(index) {
		glCreateBuffers(1, &id);
	}

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

	unsigned int GLUniformBufferIndex = 0;

	GLUniformBuffer::GLUniformBuffer(unsigned int index) : index(index) {
		glCreateBuffers(1, &id);
	}

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

	//========================================================================
	//	OpenGL Vertex Buffer Layout Class
	//========================================================================

	const unsigned int GLVertexLayout::VertexDataTypeToGLDataType[] = {
		GL_BOOL, GL_BYTE, GL_UNSIGNED_BYTE, GL_INT, GL_FLOAT
	};

	GLVertexLayout::GLVertexLayout(const std::string& name, Shader* shader, const std::vector<VertexElement>& elements) : VertexLayout(name) {
		// Create the Vertex Array Object
		glCreateVertexArrays(1, &id);
		glBindVertexArray(id);

		// Get the ID of the Shader for retrieving Vertex Attribute Locations
		unsigned int shaderID = reinterpret_cast<GLShader*>(shader)->GetShaderID();
		for (const VertexElement& e : elements) {
			int loc = glGetAttribLocation(shaderID, e.name.c_str());
			if (loc == -1)
				Logger::Warn("Invalid Buffer Element name \"" + e.name + "\" in layout: " + name);

			// Get the OpenGL representation of the data type
			unsigned int type = VertexDataTypeToGLDataType[static_cast<int>(e.type)];

			// Set the Vertex Attribute Format
			if (type == GL_DOUBLE)
				glVertexAttribLFormat(loc, e.count, type, stride);
			else if (type == GL_FLOAT || e.normalize)
				glVertexAttribFormat(loc, e.count, type, true, stride);
			else
				glVertexAttribIFormat(loc, e.count, type, stride);

			// Enable the Vertex Attribute. Change first parameter of "glVertexAttribBinding" to change the Vertex Buffer to read from (Not Implemented on VB side)
			glVertexAttribBinding(0, loc);
			glEnableVertexAttribArray(loc);
			stride += VertexDataTypeSize[static_cast<int>(e.type)] * e.count;
		}
	}

	void GLVertexLayout::Bind() const {
		glBindVertexArray(id);
	}

	void GLVertexLayout::Unbind() const {
		glBindVertexArray(0);
	}
}