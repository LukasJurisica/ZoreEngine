#pragma once
#include <vector>
#include "graphics/Shader.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Vertex Buffer Class
	//========================================================================

	class VertexBuffer {
	public:
		static VertexBuffer* Create(void* data, unsigned int size, unsigned int stride);
		virtual ~VertexBuffer() = default;

		virtual void Set(const void* data, unsigned int size, unsigned int stride = 0u) = 0;
		virtual void Update(const void* data, unsigned int size, unsigned int offset = 0u) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	//========================================================================
	//	Platform Agnostic Index Buffer Class
	//========================================================================

	typedef unsigned short Index;

	class IndexBuffer {
	public:
		static IndexBuffer* Create(void* data, unsigned int size);
		virtual ~IndexBuffer() = default;

		virtual void Set(const void* data, unsigned int size) = 0;
		virtual void Update(const void* data, unsigned int size, unsigned int offset = 0u) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	//========================================================================
	//	Platform Agnostic Instance Buffer Class
	//========================================================================

	class InstanceArrayBuffer {
	public:
		static InstanceArrayBuffer* Create(void* data, unsigned int size, unsigned int stride);
		virtual ~InstanceArrayBuffer() = default;

		virtual void Set(const void* data, unsigned int size, unsigned int stride = 0u) = 0;
		virtual void Update(const void* data, unsigned int size, unsigned int offset = 0u) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	//========================================================================
	//	Platform Agnostic Shader Storage Buffer Class
	//========================================================================

	class ShaderStorageBuffer {
	public:
		static ShaderStorageBuffer* Create(void* data, unsigned int size, unsigned int index = 0u);
		virtual ~ShaderStorageBuffer() = default;

		virtual void Set(const void* data, unsigned int size) = 0;
		virtual void Update(const void* data, unsigned int size, unsigned int offset = 0u) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	//========================================================================
	//	Platform Agnostic Uniform Buffer Class
	//========================================================================

	class UniformBuffer {
	public:
		static UniformBuffer* Create(void* data, unsigned int size, unsigned int index = 0u);
		virtual ~UniformBuffer() = default;

		virtual void Set(const void* data, unsigned int size) = 0;
		virtual void Update(const void* data, unsigned int size, unsigned int offset = 0u) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
}