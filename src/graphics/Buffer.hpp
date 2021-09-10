#pragma once
#include <vector>
#include "graphics/Shader.hpp"

#pragma warning(disable:4250)

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

	//========================================================================
	//	Platform Agnostic Vertex Buffer Layout Class
	//========================================================================

	enum class VertexDataType { BOOL, BYTE, UBYTE, INT, FLOAT };

	struct VertexElement {
		VertexElement(std::string name, VertexDataType type, unsigned int count, bool normalize = false);

		std::string name;
		VertexDataType type;
		unsigned int count;
		bool normalize;
	};

	class VertexLayout {
	public:
		static VertexLayout* Create(const std::string& name, Shader* shader, const std::vector<VertexElement>& elements);
		static VertexLayout* Get(const std::string& name);
		virtual ~VertexLayout();

		unsigned int GetStride();
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	protected:
		VertexLayout(const std::string& name);
		static const unsigned int VertexDataTypeSize[5];

		unsigned int stride;
		std::string name;
	};
}