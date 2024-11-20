#include "main.hpp"
#include <zore/core/FileManager.hpp>
#include <zore/audio/AudioEngine.hpp>
#include <zore/ui/Manager.hpp>
#include <zore/ui/Editor.hpp>
#include <zore/Debug.hpp>

using namespace zore;

static bool s_display_console = false;

DemoApplication::DemoApplication(const LaunchOptions& options) : Application(options), m_camera(Window::GetAspectRatio(), 4.f), m_main_menu(UI::Manager::CreateLayer("example_menu")) {
	FileManager::Init("/examples/01_sandbox application/");
	RenderEngine::SetVSync(false);
	m_camera.SetHeight(static_cast<float>(Window::GetSize().y));
	m_camera.SetPosition({ Window::GetSize().x >> 1, Window::GetSize().y >> 1 });

	// Initialize Action map
	UI::Manager::Bind(action_map);
	UI::Manager::Bind(m_main_menu);

	action_map.RegisterAction(ActionMap::Source::KEYBOARD, KEY_ESCAPE, true, false, [](bool start) {
		s_display_console = !s_display_console;
		});
}

void DemoApplication::Run() {
	RenderEngine::SetClearColour(1.f, 0.f, 0.f, 1.f);
	RenderEngine::SetClearMode({ BufferType::COLOUR, BufferType::DEPTH });
	RenderEngine::SetTopology(MeshTopology::TRIANGLE_STRIP);
	RenderEngine::SetDepthTest(DepthTest::LESS);

	m_ui_shader.SetSource("default_ui.glsl").Compile();
	VertexLayout layout(m_ui_shader, {}, { { "quad", VertexDataType::INT_32, 4} });
	layout.Bind();

	AudioEngine::Play("assets/screen_ring_pen_king.mp3");
	CreateSimpleUI();

	while (!Window::ShouldClose()) {
		RenderEngine::Clear();

		m_ui_shader.SetFloat4("camera", { m_camera.GetPosition(), m_camera.GetScale() * glm::vec2(1, -1) });
		m_main_menu.Render();

		Editor::BeginFrame();
		if (s_display_console)
			Console::Draw();
		//Editor::ShowDemoWindow();
		Editor::EndFrame();
		Window::Update();
	}
}

void DemoApplication::CreateSimpleUI() {
	

	/*
	UI::Style::Create("root")
		.SetColour(Colour::hex("#00FFFF"));

	UI::Style::Create("a")
		.SetSize(UI::Unit::PCT(100), UI::Unit::PCT(30))
		.SetPadding(UI::Unit::REM(2))
		.SetColour(Colour::hex("#00FF00"));

	UI::Style::Create("b")
		.SetSize(UI::Unit::PCT(100), UI::Unit::PCT(40))
		.SetPadding(UI::Unit::REM(2))
		.SetColour(Colour::hex("#0000FF"));

	UI::Style& child_style = UI::Style::Create("child")
		.SetHeight(UI::Unit::AUT(), 1.f)
		.SetMargin(UI::Unit::REM(2), UI::Unit::REM(2))
		.SetColour(Colour::hex("#FFFF00"));

	UI::Style::Clone(child_style, "child2")
		.SetMargin(UI::Unit::AUT(), UI::Unit::REM(2), UI::Unit::REM(2), UI::Unit::REM(2));

	UI::Element a(UI::Element::Type::PANEL, "a");
	UI::Element b(UI::Element::Type::PANEL, "b");
	UI::Element child(UI::Element::Type::BUTTON, "child");

	a.AddChild(child);
	a.AddChild(child);
	a.AddChild(child);

	b.AddChild(child, "child2");
	b.AddChild(child);
	b.AddChild(child);

	m_main_menu.AddChild(a);
	m_main_menu.AddChild(b);
	*/

	UI::Style& child_style = UI::Style::Create("child")
		.SetHeight(UI::Unit::PCT(90), 1.f)
		.SetMargin(UI::Unit::AUT())
		.SetColour(Colour::hex("#FFFFFF"));

	UI::Element a(UI::Element::Type::BUTTON, &child_style);

	action_map.RegisterAction(ActionMap::Source::INTERNAL, a.GetUUID(), true, true, [](bool start) {
		Logger::Log("Awesome");
		});

	m_main_menu.AddChild(a);

	glm::ivec2 resolution = Window::GetSize();
	m_main_menu.Resize(resolution.x, resolution.y);
}

bool DemoApplication::OnMouseMove(float x, float y, float dx, float dy) {
	//UI::Layer::HandleMouseMove(static_cast<uint32_t>(x), static_cast<uint32_t>(y));
	return false;
}

bool DemoApplication::OnMousePress(int button) {
	//if (uint32_t trigger = UI::Layer::HandleMouseClick(button))
	//	return action_map.HandleEvent(ActionMap::Source::INTERNAL, trigger, 1);
	//else
	return action_map.HandleEvent(ActionMap::Source::MOUSE, button, 1);
}

bool DemoApplication::OnMouseRelease(int button) {
	return action_map.HandleEvent(ActionMap::Source::MOUSE, button, 0);
}

bool DemoApplication::OnKeyPress(int key, int mods) {
	//if (uint32_t trigger = UI::Layer::HandleKeyPress(key))
	//	return action_map.HandleEvent(ActionMap::Source::INTERNAL, trigger, 1);
	//else
	return action_map.HandleEvent(ActionMap::Source::KEYBOARD, key, 1);
}

bool DemoApplication::OnKeyRelease(int key) {
	return action_map.HandleEvent(ActionMap::Source::KEYBOARD, key, 0);
}

void DemoApplication::OnWindowResize(int width, int height, float aspect_ratio) {
	m_camera.SetHeight(static_cast<float>(height));
	m_camera.SetAspectRatio(aspect_ratio);
	m_camera.SetPosition({ width >> 1, height >> 1 });
}

Application* Application::Create() {
	return new DemoApplication({
		.enable_audio = false
		});
}