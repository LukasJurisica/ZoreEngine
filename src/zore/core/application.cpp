#include "zore/core/application.hpp"
#include "zore/platform/processor.hpp"
#include "zore/io/file_manager.hpp"
#include "zore/io/commands/command_manager.hpp"
#include "zore/devices/window.hpp"
#include "zore/networking/networking_manager.hpp"
#include "zore/audio/audio_manager.hpp"
#include "zore/ui/editor.hpp"
#include "zore/debug.hpp"

namespace zore {

	//========================================================================
	//	Abstract Application Class
	//========================================================================

	static bool s_initialized = false;

	Application::Application(const LaunchOptions& options) {
		Processor::Init();
		File::Manager::Init();
		Window::Init(options.transparent_window);
		if (options.enable_networking)
			net::Manager::Init();
		if (options.enable_audio)
			audio::Manager::Init();
		Command::Manager::Init();
		Editor::Init(options.enable_multi_viewports, options.enable_docking, options.enable_keyboard_navigation);
	}

	void Application::Init(const std::vector<std::wstring>& command_line_arguments) {
		s_command_line_arguments = command_line_arguments;
		if (s_initialized)
			return;

		s_initialized = true;
		Application* app = Create();
		app->Run();
		delete app;
	}

	void Application::Cleanup() {
		Command::Manager::Clear();
		Editor::Cleanup();
		audio::Manager::Cleanup();
		net::Manager::Cleanup();
		Window::Cleanup();
		Processor::Cleanup();
		s_initialized = false;
	}
}