#include <zore/core/Application.hpp>
#include <zore/core/FileManager.hpp>
#include <zore/core/Camera.hpp>
#include <zore/ui/Editor.hpp>
#include <zore/ui/Layer.hpp>

#include <zore/Devices.hpp>
#include <zore/Graphics.hpp>
#include <zore/Debug.hpp>

#include <zore/audio/AudioEngine.hpp>
#include <zore/networking/Request.hpp>
#include <zore/networking/Socket.hpp>
#include <zore/math/vector.hpp>
#include <zore/core/ActionMap.hpp>

using namespace zore;

static bool display_console = false;

class DemoApplication : public Application, Window::Listener, Mouse::Listener, Keyboard::Listener {
public:	
	DemoApplication() : m_camera(Window::GetAspectRatio(), 4.f), m_main_menu("example_menu") {
		FileManager::Init("/examples/01_sandbox application/");
		Editor::Init({});
		RenderEngine::SetVSync(false);
		m_camera.SetHeight(static_cast<float>(Window::GetSize().y));
		m_camera.SetPosition({ Window::GetSize().x >> 1, Window::GetSize().y >> 1 });

		// Initialize Action map
		action_map.RegisterAction(ActionMap::Source::KEYBOARD, KEY_ESCAPE, true, false, [](bool start) {
			display_console = !display_console;
			});
		action_map.Bind();
	}

	void CreateSimpleUI() {
		UI::Style& root_style = UI::Style::Create("root")
			.SetFlowDirection(UI::FlowDirection::VERTICAL)
			.SetColour(Colour::hex("#FF0000"));

		UI::Style& a_style = UI::Style::Create("a")
			.SetSize(UI::Unit::PCT(100), UI::Unit::PCT(30))
			.SetPadding(UI::Unit::REM(2))
			.SetColour(Colour::hex("#00FF00"));

		UI::Style& b_style = UI::Style::Create("b")
			.SetSize(UI::Unit::PCT(100), UI::Unit::PCT(40))
			.SetPadding(UI::Unit::REM(2))
			.SetColour(Colour::hex("#0000FF"));

		UI::Style& child_style = UI::Style::Create("child")
			.SetHeight(UI::Unit::AUT(), 1.f)
			.SetMargin(UI::Unit::REM(2), UI::Unit::REM(2))
			.SetColour(Colour::hex("#FFFF00"));

		UI::Element a(UI::Element::Type::PANEL, "a");
		UI::Element b(UI::Element::Type::PANEL, "b");
		UI::Element child(UI::Element::Type::PANEL, "child");

		a.AddChild(child);
		a.AddChild(child);
		a.AddChild(child);

		b.AddChild(child);
		b.AddChild(child);
		b.AddChild(child);

		m_main_menu.AddChild(a);
		m_main_menu.AddChild(b);

		glm::ivec2 resolution = Window::GetSize();
		m_main_menu.SetStyle("root");
		m_main_menu.Flush(resolution.x, resolution.y);
		UI::Layer::Bind("example_menu");
	}

	void Run() {
		RenderEngine::SetClearColour(1.f, 0.f, 0.f, 1.f);
		RenderEngine::SetClearMode({BufferType::COLOUR, BufferType::DEPTH});
		RenderEngine::SetTopology(MeshTopology::TRIANGLE_STRIP);
		RenderEngine::SetDepthTest(DepthTest::LESS);

		//Logger::Log("Hostname: " + Socket::GetHostName());
		//PassiveSocket ps(3000, Protocol::TCP);

		//std::string html = Request::Make("https://info.cern.ch/", HTTPMethod::GET);
		//Logger::Log(html);

		//ActiveSocket as("127.0.0.1", 3000, Protocol::TCP);
		//std::vector<ActiveSocket> new_connections;
		//ps.AcceptConnections(new_connections);
		//Logger::Log(new_connections.size());
		
		m_quad_shader.SetSource("example.glsl").Compile();
		m_ui_shader.SetSource("default_ui.glsl").Compile();
		VertexLayout layout(m_ui_shader, {}, {{ "quad", VertexDataType::UINT_32, 4}});
		layout.Bind();

		CreateSimpleUI();

		AudioEngine::Play("assets/screen_ring_pen_king.mp3");

		while (!Window::ShouldClose()) {
			RenderEngine::Clear();
			
			//m_quad_shader.SetFloat4("camera", { m_camera.GetPosition(), m_camera.GetScale()});
			//RenderEngine::DrawLinear(4);

			m_ui_shader.SetFloat4("camera", { m_camera.GetPosition(), m_camera.GetScale() * glm::vec2(1, -1)});
			UI::Layer::Render();
			
			Editor::BeginFrame();
			if (display_console)
				Console::Draw();
			//Editor::ShowDemoWindow();
			Editor::EndFrame();
			Window::Update();
		}
	}

	void OnWindowResize(int width, int height, float aspect_ratio) override {
		m_camera.SetHeight(static_cast<float>(height));
		m_camera.SetAspectRatio(aspect_ratio);
		m_camera.SetPosition({ width >> 1, height >> 1 });
		UI::Layer::Flush(width, height);
		m_ui_shader.SetFloat2("window_resolution", { width, height });
	}

private:
	Camera2D m_camera;
	Shader m_quad_shader;
	Shader m_ui_shader;
	UI::Layer m_main_menu;
	ActionMap action_map;
};

Application* Application::Create() {
	return new DemoApplication;
}