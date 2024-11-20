#pragma once

namespace zore {

	//========================================================================
	//	Abstract Application Class
	//========================================================================

	class Application {
	public:
		struct LaunchOptions {
			bool enable_audio = false;
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
		Application(const LaunchOptions& options = {});
		virtual ~Application() = default;

	private:
		static Application* Create();
	};
}