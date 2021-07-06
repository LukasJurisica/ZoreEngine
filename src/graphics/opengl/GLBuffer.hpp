#pragma once
#include "graphics/Buffer.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Base Buffer Class
	//========================================================================

	class GLBuffer : public virtual Buffer {
	public:
		GLBuffer(unsigned int type);
		GLBuffer(unsigned int type, void* data, unsigned int size, unsigned int mode);
		~GLBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Set(const void* data, unsigned int size) override;
		virtual void Update(const void* data, unsigned int size, unsigned int offset) override;
		unsigned int GetBufferID() const;

	protected:
		unsigned int bufferID;
		unsigned int type;
	};

	//========================================================================
	//	OpenGL Base Block Buffer Class
	//========================================================================

	class GLBlockBuffer : public GLBuffer {
	public:
		GLBlockBuffer(unsigned int type, unsigned int index);
		GLBlockBuffer(unsigned int type, void* data, unsigned int size, unsigned int mode, unsigned int index);

		virtual void Bind() const override;
		virtual void UnBind() const override;

	protected:
		unsigned int index;
	};

	//========================================================================
	//	OpenGL Vertex Buffer Class
	//========================================================================

	class GLVertexBuffer : public VertexBuffer, public GLBuffer {
	public:
		GLVertexBuffer();
		GLVertexBuffer(void* vertices, unsigned int size);
	};

	//========================================================================
	//	OpenGL Index Buffer Class
	//========================================================================

	class GLIndexBuffer : public IndexBuffer, public GLBuffer {
	public:
		GLIndexBuffer();
		GLIndexBuffer(void* indices, unsigned int size);
	};

	//========================================================================
	//	OpenGL Shader Storage Buffer Class
	//========================================================================

	class GLShaderStorageBuffer : public ShaderStorageBuffer, public GLBlockBuffer {
	public:
		GLShaderStorageBuffer(unsigned int index);
		GLShaderStorageBuffer(void* data, unsigned int size, unsigned int index);
	};

	//========================================================================
	//	OpenGL Uniform Buffer Class
	//========================================================================

	class GLUniformBuffer : public UniformBuffer, public GLBlockBuffer {
	public:
		GLUniformBuffer(unsigned int index);
		GLUniformBuffer(void* data, unsigned int size, unsigned int index);
		void AttachToShader(Shader* shader, const std::string& name) override;
	};

	//========================================================================
	//	OpenGL Vertex Buffer Layout Class
	//========================================================================

	class GLBufferLayout : public BufferLayout {
	public:
		GLBufferLayout(const std::string& name, Shader* shader, const std::vector<BufferElement>& elements);
		void Bind() const override;
	};
}