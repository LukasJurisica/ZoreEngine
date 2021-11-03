#pragma once
#include "graphics/Buffer.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Vertex Buffer Class
	//========================================================================

	class GLVertexBuffer : public VertexBuffer {
	public:
		GLVertexBuffer(const void* data, unsigned int size, unsigned int stride);
		~GLVertexBuffer();

		void Set(const void* data, unsigned int size, unsigned int stride) override;
		void Update(const void* data, unsigned int size, unsigned int offset) override;
		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
		unsigned int stride;
	};

	//========================================================================
	//	OpenGL Index Buffer Class
	//========================================================================

	class GLIndexBuffer : public IndexBuffer {
	public:
		GLIndexBuffer(const void* data, unsigned int size);
		~GLIndexBuffer();

		void Set(const void* data, unsigned int size) override;
		void Update(const void* data, unsigned int size, unsigned int offset) override;
		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
	};

	//========================================================================
	//	OpenGL Instance Buffer Class
	//========================================================================

	class GLInstanceArrayBuffer : public InstanceArrayBuffer {
	public:
		GLInstanceArrayBuffer(const void* data, unsigned int size, unsigned int stride);
		~GLInstanceArrayBuffer();

		void Set(const void* data, unsigned int size, unsigned int stride) override;
		void Update(const void* data, unsigned int size, unsigned int offset) override;
		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
		unsigned int stride;
	};

	//========================================================================
	//	OpenGL Shader Storage Buffer Class
	//========================================================================

	class GLShaderStorageBuffer : public ShaderStorageBuffer {
	public:
		GLShaderStorageBuffer(const void* data, unsigned int size, unsigned int index);
		~GLShaderStorageBuffer();

		void Set(const void* data, unsigned int size) override;
		void Update(const void* data, unsigned int size, unsigned int offset) override;
		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
		unsigned int index;
	};

	//========================================================================
	//	OpenGL Uniform Buffer Class
	//========================================================================

	class GLUniformBuffer : public UniformBuffer {
	public:
		GLUniformBuffer(const void* data, unsigned int size, unsigned int mode, unsigned int index);
		~GLUniformBuffer();

		void Set(const void* data, unsigned int size) override;
		void Update(const void* data, unsigned int size, unsigned int offset) override;
		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
		unsigned int index;
		unsigned int usage;
	};
}