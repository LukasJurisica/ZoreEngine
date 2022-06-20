#pragma once
#include "graphics/Mesh.hpp"
#include <vector>

namespace zore {

	//========================================================================
	//	Platform Agnostic Render Engine Interface
	//========================================================================

	enum class API { NONE, OPENGL, VULKAN };
	enum class BufferType { COLOUR, DEPTH, STENCIL };
	enum class MeshTopology { POINT_LIST, LINE_LIST, LINE_STRIP, LINE_LOOP, TRIANGLE_LIST, TRIANGLE_STRIP, TRIANGLE_FAN };

	class RenderEngine {
	public:
		static void Init();
		static RenderEngine* Get();
		virtual ~RenderEngine() = default;

		static API GetAPI();
		static void SetAPI(API api);
		void Draw(Mesh* mesh, unsigned int offset = 0u);
		void DrawInstanced(Mesh* mesh, unsigned int count, unsigned int offset = 0u);

		virtual void SetViewport(unsigned int width, unsigned int height, unsigned int x = 0u, unsigned int y = 0u) = 0;
		virtual void SetBackFaceCulling(bool value) = 0;
		virtual void SetDepthTest(bool value) = 0;
		virtual void SetStencilTest(bool value) = 0;
		virtual void SetWireframe(bool value) = 0;
		virtual void SetVSync(bool value) = 0;
		virtual void SetClearColour(float r, float g, float b, float a = 1.0f) = 0;
		virtual void SetClearMode(const std::vector<BufferType>& buffers) = 0;
		virtual void SetTopology(MeshTopology topology) = 0;
		virtual void Clear() = 0;
		virtual void DrawLinear(unsigned int count, unsigned int offset = 0u) = 0;
		virtual void DrawIndexed(unsigned int count, unsigned int offset = 0u) = 0;
		virtual void DrawLinearInstanced(unsigned int vertexCount, unsigned int modelCount, unsigned int offset = 0u) = 0;
		virtual void DrawIndexedInstanced(unsigned int indexCount, unsigned int modelCount, unsigned int offset = 0u) = 0;

		//void begin();
		//void end();
		//void submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);
	};

	//========================================================================
	//	Platform Agnostic Multidraw Command Buffer Class
	//========================================================================

	struct MultidrawCommand {
		MultidrawCommand(unsigned int vertexCount, unsigned int instanceCount = 1, unsigned int vertexOffset = 0, unsigned int instanceOffset = 0);
		unsigned int vertexCount;
		unsigned int instanceCount;
		unsigned int vertexOffset;
		unsigned int instanceOffset;
	};

	class MultidrawCommandBuffer {
	public:
		static MultidrawCommandBuffer* Create(MultidrawCommand* data, unsigned int count, bool calculateInstanceOffsets = false);
		virtual ~MultidrawCommandBuffer() = default;
		static void CalculateInstanceOffsets(MultidrawCommand* data, unsigned int count, unsigned int baseOffset = 0u);

		virtual void Set(MultidrawCommand* data, unsigned int count) = 0;
		virtual void Update(MultidrawCommand* data, unsigned int count, unsigned int offset = 0u) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
}