#pragma once
#include "graphics/RenderEngine.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Render Engine
	//========================================================================

	class GLRenderEngine : public RenderEngine {
	public:
		GLRenderEngine();
		~GLRenderEngine() = default;

		void SetIndexType(IndexType type) override;
		void SetViewport(unsigned int width, unsigned int height, unsigned int x, unsigned int y) override;
		void SetFaceCulling(CullingMode mode) override;
		void SetDepthTest(bool value) override;
		void SetStencilTest(bool value) override;
		void SetWireframe(bool value) override;
		void SetVSync(bool value) override;
		void SetClearColour(float r, float g, float b, float a) override;
		void SetClearMode(const std::vector<BufferType>& buffers) override;
		void EnableColourChannels(bool r, bool g, bool b, bool a) override;
		void SetTopology(MeshTopology topology) override;
		void Clear() override;
		void DrawLinear(unsigned int count, unsigned int offset) override;
		void DrawIndexed(unsigned int count, unsigned int offset) override;
		void DrawLinearInstanced(unsigned int vertexCount, unsigned int modelCount, unsigned int offset) override;
		void DrawIndexedInstanced(unsigned int indexCount, unsigned int modelCount, unsigned int offset) override;

	private:
		unsigned int clearMode;
		unsigned int topology;
		unsigned int indexType;
		unsigned int faceCullingMode;
		bool faceCullingEnabled;
	};

	//========================================================================
	//	OpenGL Multidraw Command Buffer Class
	//========================================================================

	class GLMultidrawCommandBuffer : public MultidrawCommandBuffer {
	public:
		GLMultidrawCommandBuffer(MultidrawCommand* data, unsigned int count);
		~GLMultidrawCommandBuffer();

		void Set(MultidrawCommand* data, unsigned int count) override;
		void Update(MultidrawCommand* data, unsigned int count, unsigned int offset) override;
		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
	};
}