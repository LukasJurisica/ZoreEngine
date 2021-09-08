#pragma once
#include "graphics/Mesh.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Render Engine Interface
	//========================================================================

	enum class API { None, OpenGL, Vulkan };
	enum class BufferType { COLOUR, DEPTH, STENCIL };
	enum class MeshTopology { Point, Line, LineStrip, TriangleList, TriangleStrip, TriangleFan };

	class RenderEngine {
		friend class Window;
	public:
		static RenderEngine* Get();
		virtual ~RenderEngine() = default;

		static API GetAPI();
		static void SetAPI(API api);
		void Draw(Mesh* m, unsigned int offset = 0u);

		virtual void SetViewport(unsigned int width, unsigned int height, unsigned int x = 0u, unsigned int y = 0u) = 0;
		virtual void SetBackFaceCulling(bool value) = 0;
		virtual void SetWireframe(bool value) = 0;
		virtual void SetVSync(bool value) = 0;
		virtual void SetClearColour(float r, float g, float b, float a = 1.0f) = 0;
		virtual void SetClearMode(const std::vector<BufferType>& buffers) = 0;
		virtual void Clear() = 0;
		virtual void DrawLinear(unsigned int count, unsigned int offset = 0u) = 0;
		virtual void DrawIndexed(unsigned int count, unsigned int offset = 0u) = 0;

		//void begin();
		//void end();
		//void submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

	private:
		static void Init();
	};
}