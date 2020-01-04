#include "graphics/opengl/OpenGLBuffer.hpp"

#include <glad/glad.h>

//////////////////////////
// OpenGL Vertex Buffer //
//////////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void OpenGLVertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
}

void OpenGLVertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
	glDeleteBuffers(1, &m_bufferID);
}

/////////////////////////
// OpenGL Index Buffer //
/////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_count(count) {
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

void OpenGLIndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void OpenGLIndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
	glDeleteBuffers(1, &m_bufferID);
}

/////////////////////////
// OpenGL Vertex Array //
/////////////////////////

static GLenum SDTtoGLDT(ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Int:
		return GL_INT;
	case ShaderDataType::Float:
		return GL_FLOAT;
	case ShaderDataType::Mat:
		return GL_FLOAT;
	case ShaderDataType::Bool:
		return GL_BOOL;
	default:
		return GL_FALSE;
	}
}

OpenGLVertexArray::OpenGLVertexArray() {
	glGenVertexArrays(1, &m_arrayID);
}

void OpenGLVertexArray::bind() const {
	glBindVertexArray(m_arrayID);
}

void OpenGLVertexArray::unbind() const {
	glBindVertexArray(0);
}

void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
	glBindVertexArray(m_arrayID);
	vertexBuffer->bind();
	const std::shared_ptr<BufferLayout>& layout = vertexBuffer->getLayout();

	int index = 0;
	for (const BufferElement& element : layout->getElements()) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.count, SDTtoGLDT(element.type), element.normalized, layout->getStride(), (const void*)element.offset);
		index++;
	}
	m_vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
	glBindVertexArray(m_arrayID);
	indexBuffer->bind();
	m_indexBuffer = indexBuffer;
}

OpenGLVertexArray::~OpenGLVertexArray() {
	glDeleteVertexArrays(1, &m_arrayID);
}