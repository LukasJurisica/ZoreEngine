#include "zore/core/Application.hpp"
#include "zore/utils/FileManager.hpp"
#include "zore/devices/Window.hpp"
#include "zore/ui/EditorUI.hpp"
#include "zore/debug/Debug.hpp"

namespace zore {

	static bool initialized = false;

	void Application::Init() {
		if (!initialized) {
			initialized = true;
			FileManager::Init();
			Window::Init(0, 0);

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