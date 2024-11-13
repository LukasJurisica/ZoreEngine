#include "zore/core/Application.hpp"
#include "zore/platform/Processor.hpp"
#include "zore/core/FileManager.hpp"
#include "zore/devices/Window.hpp"
#include "zore/audio/AudioEngine.hpp"
#include "zore/ui/Editor.hpp"
#include "zore/Debug.hpp"

namespace zore {

	static bool initialized = false;

	Application::Application(bool transparent_window) {
		Processor::Init();
		FileManager::Init();
		Window::Init(transparent_window);
		AudioEngine::Init();
		Console::RegisterCommand("help", Console::Help);
	}

	void Application::Init() {
		if (initialized)
			return;

		initialized = true;
		Application* app = Create();
		app->Run();
		delete app;
	}

	void Application::Cleanup() {
		Console::UnregisterAllCommands();
		Editor::Cleanup();
		AudioEngine::Cleanup();
		Window::Cleanup();
		Processor::Cleanup();
		initialized = false;
	}
}