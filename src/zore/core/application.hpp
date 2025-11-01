#pragma once

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
		static void Init();
		virtual void Run() = 0;
		static void Cleanup();

	protected:
		Application(const LaunchOptions& options = { 
            .enable_audio = false,
            .enable_networking = false,
            .transparent_window = false,
            .enable_editor_ui = true,
            .enable_docking = false,
            .enable_multi_viewports = false,
            .enable_keyboard_navigation = false
        });
		virtual ~Application() = default;

	private:
		static Application* Create();
	};
}