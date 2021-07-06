#pragma once
#include "graphics/RenderEngine.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Render Engine
	//========================================================================

	class GLRenderEngine : public RenderEngine {
	public:
		static void Init();
		GLRenderEngine();
		~GLRenderEngine() = default;

		void SetClearColour(float r, float g, float b, float a) override;
		void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
		void Clear() override;
		void DrawLinear(unsigned int count, unsigned int offset) override;
		void DrawIndexed(unsigned int count, unsigned int offset) override;
	};
}