#include <zore/core/Application.hpp>
#include <zore/devices/Window.hpp>
#include <zore/graphics/RenderEngine.hpp>
#include <zore/ui/EditorUI.hpp>

namespace zore {

	class DemoApplication : public Application {
	public:
		DemoApplication() {
			EditorUI::Init(false);
		}

		void Run() {
			RenderEngine::SetClearColour(1.0, 0.0, 0.0, 1.0);

			while (!Window::ShouldClose()) {
				RenderEngine::Clear();

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