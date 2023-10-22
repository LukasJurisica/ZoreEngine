#include <zore/core/Application.hpp>
#include <zore/devices/Window.hpp>
#include <zore/graphics/RenderEngine.hpp>
#include <zore/ui/EditorUI.hpp>

#include <zore/debug/Debug.hpp>
#include <zore/utils/Timer.hpp>

#include <chrono>
#include <random>
#include <numeric>

namespace zore {

	class DemoApplication : public Application {
	public:
		DemoApplication() {
			EditorUI::Init(false);
			RenderEngine::SetVSync(false);
		}

		void Run() {
			RenderEngine::SetClearColour(1.0, 0.0, 0.0, 1.0);

			Timer t;
			int frame_count = 0;

			while (!Window::ShouldClose()) {
				RenderEngine::Clear();

				if (t.DeltaTime(false) > 1.f) {
					Logger::Log(frame_count);
					frame_count = 0;
				}
				frame_count++;

				EditorUI::BeginFrame();
				EditorUI::ShowDemoWindow();
				EditorUI::EndFrame();
				Window::Update();
			}
		}

	private:

	};

	Application* Application::Create() {
		return new DemoApplication;
	}
}