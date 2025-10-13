#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/span.hpp"

namespace zore {

	//========================================================================
	//	Vertex Buffer Class
	//========================================================================

	class VertexBuffer {
	public:
		VertexBuffer(bool instanced = false);
		VertexBuffer(const VoidSpan& span, bool instanced = false);
		VertexBuffer(const void* data, uint32_t size, uint32_t stride, bool instanced = false);
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&&) = delete;
		~VertexBuffer();

		uint32_t GetID() const;
		void Set(const VoidSpan& span);
		void Set(const void* data, uint32_t size, uint32_t stride = 0u);
		void Update(const void* data, uint32_t size, uint32_t offset = 0u);
		void Bind() const;

	private:
		uint32_t m_id;
		uint32_t m_stride;
		uint32_t m_index;
	};

	//========================================================================
	//	Index Buffer Class
	//========================================================================

	class IndexBuffer {
	public:
		IndexBuffer();
		IndexBuffer(const VoidSpan& span);
		IndexBuffer(const void* data, uint32_t size);
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&&) = delete;
		~IndexBuffer();

		uint32_t GetID() const;
		void Set(const VoidSpan& span);
		void Set(const void* data, uint32_t size);
		void Update(const void* data, uint32_t size, uint32_t offset = 0u);
		void Bind() const;

	private:
		uint32_t m_id;
	};

	//========================================================================
	//	Shader Storage Buffer Class
	//========================================================================

	class ShaderStorageBuffer {
	public:
		ShaderStorageBuffer();
		ShaderStorageBuffer(const VoidSpan& span);
		ShaderStorageBuffer(const void* data, uint32_t size);
		ShaderStorageBuffer(const ShaderStorageBuffer&) = delete;
		ShaderStorageBuffer(ShaderStorageBuffer&&) = delete;
		ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;
		ShaderStorageBuffer& operator=(ShaderStorageBuffer&&) = delete;
		~ShaderStorageBuffer();

		uint32_t GetID() const;
		void Set(const VoidSpan& span);
		void Set(const void* data, uint32_t size);
		void Update(const void* data, uint32_t size, uint32_t offset);
		void Bind() const;
		void Bind(uint32_t index);

	private:
		uint32_t m_id;
		uint32_t m_index;
	};

	//========================================================================
	//	Uniform Buffer Class
	//========================================================================

	class UniformBuffer {
	public:
		UniformBuffer();
		UniformBuffer(const VoidSpan& span);
		UniformBuffer(const void* data, uint32_t size);
		UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer(UniformBuffer&&) = delete;
		UniformBuffer& operator=(const UniformBuffer&) = delete;
		UniformBuffer& operator=(UniformBuffer&&) = delete;
		~UniformBuffer();

		uint32_t GetID() const;
		void Set(const VoidSpan& span);
		void Set(const void* data, uint32_t size);
		void Update(const void* data, uint32_t size, uint32_t offset = 0u);
		void Bind() const;
		void Bind(uint32_t index);

	private:
		uint32_t m_id;
		uint32_t m_index;
	};
}