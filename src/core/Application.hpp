#pragma once
#include "core/Camera.hpp"
#include "devices/Window.hpp"
#include "graphics/RenderEngine.hpp"
#include "graphics/FrameBuffer.hpp"

namespace zore {

	struct ShaderData {
		glm::mat4 vp_mat;
		glm::vec3 cameraPos;
		float time;
	};

	class Application : WindowListener, KeyListener {
	public:
		Application();
		~Application() = default;
		static void Init();
		static void Cleanup();
		void Run();

	protected:
		void OnWindowResize(int width, int height) override;
		void OnKeyPress(unsigned char key, int mods) override;

	private:
		Window window;
		RenderEngine* engine;
		Camera camera;
		FrameBuffer* frameBuffer;
	};
}