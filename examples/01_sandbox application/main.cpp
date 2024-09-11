#include <zore/core/Application.hpp>
#include <zore/core/FileManager.hpp>
#include <zore/core/Camera.hpp>
#include <zore/ui/EditorUI.hpp>
#include <zore/ui/UILayer.hpp>
#include <zore/utils/Colour.hpp>

#include <zore/Devices.hpp>
#include <zore/Graphics.hpp>
#include <zore/Debug.hpp>

#include <zore/math/vector.hpp>

namespace zore {

	class DemoApplication : public Application, Window::Listener, Mouse::Listener {
	public:	
		DemoApplication() : m_camera(Window::GetAspectRatio(), 4.f) {
			FileManager::Init("/examples/01_sandbox application/");
			EditorUIParams params;
			EditorUI::Init(params);
			RenderEngine::SetVSync(false);
			m_camera.SetHeight(m_scale);

			glm::ivec2 resolution = Window::GetSize();
			m_main_menu.Update(resolution.x, resolution.y);
		}

		void CreateSimpleUI() {
			UIElement elem;
			elem.SetWidth(Unit::PCT(50));

			//m_main_menu.AddChild(button);
			m_main_menu.AddChild(elem);
		}

		void Run() {
			RenderEngine::SetClearColour(1.f, 0.f, 0.f, 1.f);
			RenderEngine::SetTopology(MeshTopology::TRIANGLE_STRIP);

			//Logger::Log("Hostname: " + Socket::GetHostName());
			//PassiveSocket ps(3000, Protocol::TCP);

			//std::string html = Request::Make("https://info.cern.ch/", HTTPMethod::GET);
			//Logger::Log(html);

			//ActiveSocket as("127.0.0.1", 3000, Protocol::TCP);
			//std::vector<ActiveSocket> new_connections;
			//ps.AcceptConnections(new_connections);
			//Logger::Log(new_connections.size());


			Logger::Log("Hello There general kenobi, how are you", "This is insane", 5);
			//BetterLogger::Log(4, 5);


			zm::vec2 x(1, 2);



			Colour c = Colour::hex("#0F0");
			Logger::Log(VEC4TOSTR(c.nrgba()));

			VertexLayout layout;

			m_shader.SetSource("example.glsl").Compile();

			while (!Window::ShouldClose()) {
				RenderEngine::Clear();

				m_shader.SetFloat4("camera", { m_camera.GetScale(), m_camera.GetPosition() });

				RenderEngine::DrawLinear(4);

				EditorUI::BeginFrame();
				Console::Draw();
				EditorUI::ShowDemoWindow();
				EditorUI::EndFrame();
				Window::Update();
			}
		}

		bool OnMouseMove(float nx, float ny, float dx, float dy) override {
			if (Mouse::GetButton(MOUSE_BUTTON_LEFT)) {
				m_camera.TranslatePixels({ -dx, dy });
			}
			return true;
		}

		bool OnMouseScroll(float dx, float dy) override {
			m_scale -= dy * 0.1f * m_scale;
			m_camera.SetHeight(m_scale);
			return true;
		}

		void OnWindowResize(int width, int height, float aspect_ratio) override {
			m_camera.SetAspectRatio(aspect_ratio);
			m_main_menu.Update(width, height);
		}

	private:
		Camera2D m_camera;
		Shader m_shader;
		float m_scale = 4.f;
		UILayer m_main_menu;
	};

	Application* Application::Create() {
		return new DemoApplication;
	}
}