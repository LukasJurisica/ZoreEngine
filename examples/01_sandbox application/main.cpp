#include <zore/core/Application.hpp>
#include <zore/devices/Window.hpp>
#include <zore/graphics/RenderEngine.hpp>
#include <zore/ui/EditorUI.hpp>

#include <zore/networking/Socket.hpp>
#include <zore/networking/Request.hpp>

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
			RenderEngine::SetClearColour(1.0, 0.0, 0.0, 0.0);

			Timer t;
			int frame_count = 0;

			//Logger::Log("Hostname: " + Socket::GetHostName());
			//PassiveSocket ps(3000, Protocol::TCP);

			//std::string html = Request::Make("https://info.cern.ch/", HTTPMethod::GET);
			//Logger::Log(html);

			//ActiveSocket as("127.0.0.1", 3000, Protocol::TCP);
			//std::vector<ActiveSocket> new_connections;
			//ps.AcceptConnections(new_connections);
			//Logger::Log(new_connections.size());

			while (!Window::ShouldClose()) {
				RenderEngine::Clear();

				if (t.DeltaTime(false) > 1.f) {
					//Logger::Log(frame_count);
					frame_count = 0;
					t.DeltaTime(true);
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
		Window::PreSetTransparent(true);
		return new DemoApplication;
	}
}