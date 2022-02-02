#pragma once
#include "core/Camera.hpp"
#include "devices/Window.hpp"
#include "devices/Keyboard.hpp"
#include "graphics/RenderEngine.hpp"
#include "graphics/FrameBuffer.hpp"
#include "graphics/Shader.hpp"

namespace zore {

	struct ShaderData {
		glm::mat4 vp_mat;
		glm::vec3 cameraPos;
		float time;
	};

	class Application : WindowListener, KeyListener {
	public:
		Application();
		~Application();
		static void Init();
		static void Cleanup();
		void Run();

	protected:
		void OnWindowResize(int width, int height) override;
		void OnKeyPress(int key, int mods) override;

	private:
		Camera camera;

		RenderEngine* engine;
		FrameBuffer* frameBuffer;
		Shader* postProcessShader;
		Shader* blockShader;
		Shader* spriteShader;
		Shader* debugLineShader;
	};
}