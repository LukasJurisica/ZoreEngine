#pragma once
#include "graphics/Buffer.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Vertex Buffer Class
	//========================================================================

	class GLVertexBuffer : public VertexBuffer {
	public:
		GLVertexBuffer();
		GLVertexBuffer(void* data, unsigned int size, unsigned int stride);
		~GLVertexBuffer();

		void Set(const void* data, unsigned int size, unsigned int stride) override;
		void Update(const void* data, unsigned int size, unsigned int offset = 0) override;
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
		GLIndexBuffer();
		GLIndexBuffer(void* data, unsigned int size);
		~GLIndexBuffer();

		void Set(const void* data, unsigned int size) override;
		void Update(const void* data, unsigned int size, unsigned int offset = 0) override;
		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
	};

	//========================================================================
	//	OpenGL Shader Storage Buffer Class
	//========================================================================

	class GLShaderStorageBuffer : public ShaderStorageBuffer {
	public:
		GLShaderStorageBuffer(unsigned int index);
		GLShaderStorageBuffer(void* data, unsigned int size, unsigned int index);
		~GLShaderStorageBuffer();

		void Set(const void* data, unsigned int size) override;
		void Update(const void* data, unsigned int size, unsigned int offset = 0) override;
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
		GLUniformBuffer(unsigned int index);
		GLUniformBuffer(void* data, unsigned int size, unsigned int index);
		~GLUniformBuffer();

		void Set(const void* data, unsigned int size) override;
		void Update(const void* data, unsigned int size, unsigned int offset = 0) override;
		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
		unsigned int index;
	};

	//========================================================================
	//	OpenGL Vertex Buffer Layout Class
	//========================================================================

	class GLVertexLayout : public VertexLayout {
	public:
		GLVertexLayout(const std::string& name, Shader* shader, const std::vector<VertexElement>& elements);
		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
		static const unsigned int VertexDataTypeToGLDataType[5];
	};
}