#include "zore/core/Application.hpp"
#include "zore/utils/FileManager.hpp"
#include "zore/devices/Window.hpp"
#include "zore/ui/EditorUI.hpp"
#include "zore/debug/Debug.hpp"

namespace zore {

	void Application::Init() {
		static bool initialized = false;
		ENSURE(!initialized, "The function Application::Init() should not be invoked by the client application");

		initialized = true;
		FileManager::Init();
		Window::Init(0, 0);

		Application* app = Create();
		app->Run();
		delete app;

		EditorUI::Cleanup();
		Window::Cleanup();
	}
}