#pragma once
#include <vector>
#include "graphics/Shader.hpp"

#pragma warning(disable:4250)

namespace zore {

	typedef unsigned short Index;

	//========================================================================
	//	Platform Agnostic GPU Buffer Base Class
	//========================================================================

	class Buffer {
	public:
		virtual ~Buffer() = default;
		virtual void Set(const void* data, unsigned int size) = 0;
		virtual void Update(const void* data, unsigned int size, unsigned int offset = 0) = 0;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};

	//========================================================================
	//	Platform Agnostic Vertex Buffer Class
	//========================================================================

	class VertexBuffer : public virtual Buffer {
	public:
		static VertexBuffer* Create(void* vertices, unsigned int size);
		virtual ~VertexBuffer() = default;
	};

	//========================================================================
	//	Platform Agnostic Index Buffer Class
	//========================================================================

	class IndexBuffer : public virtual Buffer {
	public:
		static IndexBuffer* Create(void* indices, unsigned int size);
		virtual ~IndexBuffer() = default;
	};

	//========================================================================
	//	Platform Agnostic Shader Storage Buffer Class
	//========================================================================

	class ShaderStorageBuffer : public virtual Buffer {
	public:
		static ShaderStorageBuffer* Create(void* data, unsigned int size, unsigned int index);
		virtual ~ShaderStorageBuffer() = default;
	};

	//========================================================================
	//	Platform Agnostic Uniform Buffer Class
	//========================================================================

	class UniformBuffer : public virtual Buffer {
	public:
		static UniformBuffer* Create(void* data, unsigned int size, unsigned int index);
		virtual ~UniformBuffer() = default;
		virtual void AttachToShader(Shader* shader, const std::string& name) = 0;
	};

	//========================================================================
	//	Platform Agnostic Vertex Buffer Layout Class
	//========================================================================

	struct BufferElement {
		BufferElement(std::string name, ShaderDataType type, unsigned int count, bool normalized);

		std::string name;
		ShaderDataType type;
		unsigned int count;
		unsigned int size;
		unsigned int offset;
		int attribLocation;
		bool normalized;
	};

	class BufferLayout {
	public:
		static BufferLayout* Create(const std::string& name, Shader* shader, const std::vector<BufferElement>& elements);
		virtual ~BufferLayout();
		static BufferLayout* Get(const std::string& name);
		unsigned int GetStride() const;
		virtual void Bind() const = 0;

	protected:
		BufferLayout(const std::string& name, const std::vector<BufferElement>& elements);
		std::vector<BufferElement> elements;
		uint32_t stride;

	private:
		std::string name;
	};
}