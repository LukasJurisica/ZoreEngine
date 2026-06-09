#pragma once

#include <vector>
#include <string>

namespace zore {

	//========================================================================
	//	Abstract Application Class
	//========================================================================

	class Application {
	public:
		struct LaunchOptions {
			bool enable_audio = false;
			bool enable_networking = false;
			bool transparent_window = false;

			// Editor UI options
			bool enable_editor_ui = true;
			bool enable_docking = false;
			bool enable_multi_viewports = false;
			bool enable_keyboard_navigation = false;
		};

	public:
		static void Init(const std::vector<std::wstring>& command_line_arguments = {});
		virtual void Run() = 0;
		static void Cleanup();
		static const std::vector<std::wstring>& GetArguments() { return s_command_line_arguments; }

	protected:
		Application(const LaunchOptions& options = {});
		virtual ~Application() = default;

	protected:
		static inline std::vector<std::wstring> s_command_line_arguments;

	private:
		static Application* Create();
	};
}