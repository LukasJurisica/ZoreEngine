#pragma once

namespace zore {

	class Application {
	public:
		static void Init();
		virtual void Run() = 0;
		static void Cleanup();

	protected:
		Application() = default;
		virtual ~Application() = default;

	private:
		static Application* Create();
	};
}