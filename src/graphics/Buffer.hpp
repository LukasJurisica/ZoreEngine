#pragma once

#include <cstdint>
#include <string>
#include <vector>

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
	BufferLayout(const std::initializer_list<BufferElement>& elements);

	inline const std::vector<BufferElement>& getElements() const { return m_elements; }
	inline const uint32_t getStride() { return m_stride; }

	std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

private:
	std::vector<BufferElement> m_elements;
	uint32_t m_stride;
};

class VertexBuffer {
public:
	virtual ~VertexBuffer() {}
	virtual void bind() const = 0;
	virtual void setLayout(const BufferLayout& layout) = 0;
	virtual const BufferLayout& getLayout() const = 0;
	static VertexBuffer* create(float* vertices, uint32_t size);
};

class IndexBuffer {
public:
	virtual ~IndexBuffer() {}
	virtual void bind() const = 0;
	virtual uint32_t getCount() const = 0;
	static IndexBuffer* create(uint32_t* indices, uint32_t count);
};