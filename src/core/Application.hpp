#pragma once
#include "devices/Window.hpp"
#include "graphics/RenderEngine.hpp"

namespace zore {

	class Application : WindowListener {
	public:
		Application();
		~Application() = default;
		static void EntryPoint();
		void Run();

		void OnWindowResize(int width, int height) override;

	private:
		Window window;
		RenderEngine* engine;
	};
}