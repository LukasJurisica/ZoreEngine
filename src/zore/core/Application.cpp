#include "zore/core/Application.hpp"
#include "zore/platform/Processor.hpp"
#include "zore/core/FileManager.hpp"
#include "zore/devices/Window.hpp"
#include "zore/audio/AudioEngine.hpp"
#include "zore/ui/Editor.hpp"
#include "zore/Debug.hpp"

namespace zore {

	//========================================================================
	//	Abstract Application Class
	//========================================================================

	static bool s_initialized = false;

	Application::Application(const LaunchOptions& options) {
		Processor::Init();
		FileManager::Init();
		Window::Init(options.transparent_window);
		if (options.enable_audio)
			AudioEngine::Init();
		Console::RegisterCommand("help", Console::Help);
	}

	void Application::Init() {
		if (s_initialized)
			return;

		s_initialized = true;
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
		s_initialized = false;
	}
}