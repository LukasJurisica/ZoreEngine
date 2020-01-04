#pragma once

#include "graphics/Buffer.hpp"

class OpenGLVertexBuffer : public VertexBuffer {
public:
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	~OpenGLVertexBuffer();

	void bind() const override;
	void unbind() const override;
	inline void setLayout(const std::shared_ptr<BufferLayout>& bufferLayout) override { m_bufferLayout = bufferLayout; }
	inline const std::shared_ptr<BufferLayout>& getLayout() const override { return m_bufferLayout; }
	
private:
	uint32_t m_bufferID;
	std::shared_ptr<BufferLayout> m_bufferLayout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
	~OpenGLIndexBuffer();

	void bind() const override;
	void unbind() const override;
	inline const uint32_t getCount() const override { return m_count; }

private:
	uint32_t m_bufferID;
	uint32_t m_count;
};

class OpenGLVertexArray : public VertexArray {
public:
	OpenGLVertexArray();
	~OpenGLVertexArray();

	void bind() const override;
	void unbind() const override;
	void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
	void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
	inline const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override { return m_vertexBuffers; }
	inline const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override { return m_indexBuffer; }

private:
	uint32_t m_arrayID;
	std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
	std::shared_ptr<IndexBuffer> m_indexBuffer;
};