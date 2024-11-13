#pragma once

namespace zore {

	//========================================================================
	//	Abstract Application Class
	//========================================================================

	class Application {
	public:
		struct LaunchOptions {
			LaunchOptions() = default;
			bool transparent_window = false;
			bool enable_audio = false;
		};

	public:
		static void Init();
		virtual void Run() = 0;
		static void Cleanup();

	protected:
		Application(const LaunchOptions& options = LaunchOptions());
		virtual ~Application() = default;

	private:
		static Application* Create();
	};
}