#pragma once
#include "graphics/Mesh.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Render Engine Interface
	//========================================================================

	enum class API {
		None, OpenGL, Vulkan
	};

	class RenderEngine {
	public:
		static RenderEngine* Create();
		virtual ~RenderEngine() = default;

		static void Init();
		static API GetApi();
		static void SetApi(API api);

		virtual void SetClearColour(float r, float g, float b, float a = 1.0f) = 0;
		virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
		virtual void Clear() = 0;
		virtual void Draw(Mesh* m);
		virtual void DrawLinear(unsigned int count, unsigned int offset = 0) = 0;
		virtual void DrawIndexed(unsigned int count, unsigned int offset = 0) = 0;

		//void begin();
		//void end();
		//void submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

	private:
		static bool VerificaitonInit();
	};
}