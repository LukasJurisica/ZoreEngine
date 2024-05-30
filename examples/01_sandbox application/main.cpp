#include <zore/core/Application.hpp>
#include <zore/core/FileManager.hpp>
#include <zore/core/Camera.hpp>
#include <zore/devices/Window.hpp>
#include <zore/graphics/RenderEngine.hpp>
#include <zore/graphics/VertexLayout.hpp>
#include <zore/graphics/Shader.hpp>
#include <zore/ui/EditorUI.hpp>

#include <zore/debug/Debug.hpp>
#include <zore/utils/Time.hpp>

#include <chrono>
#include <random>
#include <numeric>

#include "soloud.h"
#include "soloud_audiosource.h"
#include "soloud_wav.h"

namespace zore {

	class DemoApplication : public Application {
	public:	
		DemoApplication() {
			FileManager::Init("/examples/01_sandbox application/");
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


			SoLoud::Soloud* s_soloud_engine = new SoLoud::Soloud();
			SoLoud::Wav gWave;

			s_soloud_engine->init();

			gWave.load("examples/01_sandbox application/assets/shall_not_cast.ogg");
			int x = s_soloud_engine->play(gWave);
			s_soloud_engine->setPan(x, -0.2f);





			
			Camera2D camera(Window::GetAspectRatio(), 2.f);

			Shader shader("example.glsl");
			VertexLayout layout(shader, {});

			shader.Bind();
			layout.Bind();

			shader.SetFloat4("camera", { camera.GetScale(), camera.GetPosition() });

			RenderEngine::SetTopology(MeshTopology::TRIANGLE_STRIP);

			while (!Window::ShouldClose()) {
				RenderEngine::Clear();

				if (t.TimeHasElapsed(1.f, true)) {
					Logger::Log(frame_count);
					frame_count = 0;
				}
				frame_count++;

				RenderEngine::DrawLinear(4);

				//EditorUI::BeginFrame();
				//EditorUI::ShowDemoWindow();
				//EditorUI::EndFrame();
				Window::Update();
			}
		}

	private:

	};

	Application* Application::Create() {
		return new DemoApplication;
	}
}