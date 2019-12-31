#pragma once

#include "graphics/Buffer.hpp"

class OpenGLVertexBuffer : public VertexBuffer {
public:
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	~OpenGLVertexBuffer();

	void bind() const override;
	inline void setLayout(const BufferLayout& layout) override { m_layout = layout; }
	inline const BufferLayout& getLayout() const override { return m_layout; }
	
private:
	uint32_t m_bufferID;
	BufferLayout m_layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
	~OpenGLIndexBuffer();

	void bind() const override;
	inline uint32_t getCount() const override { return m_count; }

private:
	uint32_t m_bufferID;
	uint32_t m_count;
};