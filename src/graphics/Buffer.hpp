#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "utils/Logger.hpp"

enum class ShaderDataType {
	Int, Float, Mat, Bool
};

static uint32_t shaderDataTypeSize(ShaderDataType type, uint32_t count) {
	switch (type) {
	case ShaderDataType::Int:
		return (4 * count);
	case ShaderDataType::Float:
		return (4 * count);
	case ShaderDataType::Mat:
		return (4 * count * count);
	case ShaderDataType::Bool:
		return (1 * count);
	default:
		Logger::error("Invalid ShaderDataType"); return (0);
	}
}

struct BufferElement {
	ShaderDataType type;
	uint32_t count;
	uint32_t offset;
	uint32_t size;
	std::string name;
	bool normalized;
	BufferElement(ShaderDataType type, uint32_t count, std::string name, bool normalized) : type(type), count(count), name(name), normalized(normalized), offset(0), size(shaderDataTypeSize(type, count)) {
		if (count > 4 || count < 1)
			Logger::error("Invalid number of components for BufferElement");
	}
};

class BufferLayout {
public:
	BufferLayout();
	BufferLayout(const std::vector<BufferElement>& elements);

	inline const std::vector<BufferElement> getElements() const { return m_elements; }
	inline const uint32_t getStride() const { return m_stride; }

private:
	std::vector<BufferElement> m_elements;
	uint32_t m_stride;
};

class VertexBuffer {
public:
	virtual ~VertexBuffer() = default;
	virtual void bind() const = 0;
	virtual void unbind() const = 0;
	virtual void setLayout(const std::shared_ptr<BufferLayout>& bufferLayout) = 0;
	virtual const std::shared_ptr<BufferLayout>& getLayout() const = 0;
	static VertexBuffer* create(float* vertices, uint32_t size);
};

class IndexBuffer {
public:
	virtual ~IndexBuffer() = default;
	virtual void bind() const = 0;
	virtual void unbind() const = 0;
	virtual const uint32_t getCount() const = 0;
	static IndexBuffer* create(uint32_t* indices, uint32_t count);
};

class VertexArray {
public:
	virtual ~VertexArray() = default;
	virtual void bind() const = 0;
	virtual void unbind() const = 0;
	virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
	virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
	virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
	virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;
	static VertexArray* create();
};