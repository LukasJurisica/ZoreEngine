#include "zore/core/Application.hpp"
#include "zore/core/FileManager.hpp"
#include "zore/devices/Window.hpp"
#include "zore/ui/EditorUI.hpp"
#include "zore/Debug.hpp"

namespace zore {

	static bool initialized = false;

	Application::Application(bool transparent_window) {
		Window::Init(transparent_window);
	}

	void Application::Init() {
		if (!initialized) {
			initialized = true;
			FileManager::Init();
			
			Application* app = Create();
			app->Run();
			delete app;
		}
	}

	void Application::Cleanup() {
		EditorUI::Cleanup();
		Window::Cleanup();
		initialized = false;
	}
}