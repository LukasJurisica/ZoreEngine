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

#include <zore/structures/CirclularBuffer.hpp>

namespace zore {

	class DemoApplication : public Application {
	public:
		DemoApplication() {
			EditorUI::Init();
			RenderEngine::SetVSync(false);
		}

		void Run() {
			RenderEngine::SetClearColour(1.f, 0.f, 0.f, 1.f);

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

				if (t.TimeHasElapsed(1.f, true)) {
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