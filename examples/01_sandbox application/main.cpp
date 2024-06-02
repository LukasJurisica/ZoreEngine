#include <zore/core/Application.hpp>
#include <zore/core/FileManager.hpp>
#include <zore/core/Camera.hpp>
#include <zore/devices/Window.hpp>
#include <zore/graphics/RenderEngine.hpp>
#include <zore/graphics/VertexLayout.hpp>
#include <zore/graphics/Shader.hpp>
#include <zore/ui/EditorUI.hpp>
#include <zore/devices/Mouse.hpp>

#include <zore/debug/Debug.hpp>
#include <zore/utils/Time.hpp>

#include <chrono>
#include <random>
#include <numeric>

#include "soloud.h"
#include "soloud_wav.h"

namespace zore {

	class DemoApplication : public Application, Mouse::Listener {
	public:	
		DemoApplication() : m_camera(Window::GetAspectRatio(), 4.f) {
			FileManager::Init("/examples/01_sandbox application/");
			EditorUI::Init();
			RenderEngine::SetVSync(false);
			m_camera.SetHeight(m_scale);
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

			m_shader.SetSource("example.glsl").Compile();
			VertexLayout layout(m_shader, {});

			m_shader.Bind();
			layout.Bind();

			m_shader.SetFloat4("camera", { m_camera.GetScale(), m_camera.GetPosition() });

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

		bool OnMouseMove(float nx, float ny, float dx, float dy) override {
			if (Mouse::GetButton(MOUSE_BUTTON_LEFT)) {
				m_camera.TranslatePixels({ -dx, dy });
				m_shader.SetFloat4("camera", { m_camera.GetScale(), m_camera.GetPosition() });
			}
			return true;
		}

		bool OnMouseScroll(float dx, float dy) override {
			m_scale -= dy * 0.1f * m_scale;
			m_camera.SetHeight(m_scale);
			m_shader.SetFloat4("camera", { m_camera.GetScale(), m_camera.GetPosition() });
			return true;
		}

	private:
		Camera2D m_camera;
		Shader m_shader;
		float m_scale = 4.f;
	};

	Application* Application::Create() {
		return new DemoApplication;
	}
}