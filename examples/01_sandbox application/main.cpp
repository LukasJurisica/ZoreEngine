#include "main.hpp"
#include <zore/core/FileManager.hpp>
#include <zore/utils/ConfigManager.hpp>
#include <zore/audio/Manager.hpp>
#include <zore/ui/Manager.hpp>
#include <zore/ui/Editor.hpp>
#include <zore/ui/Font.hpp>
#include <zore/Debug.hpp>

#include <zore/networking/Address.hpp>

using namespace zore;

static bool s_display_console = false;
static bool s_quit = false;
static DemoApplication* s_instance = nullptr;

DemoApplication::DemoApplication(const LaunchOptions& options) : Application(options), m_camera(Window::GetAspectRatio(), 4.f) {
	FileManager::Init("/examples/01_sandbox application/");
	RenderEngine::SetVSync(false);
	m_camera.SetHeight(static_cast<float>(Window::GetSize().y));
	m_camera.SetPosition({ Window::GetSize().x >> 1, Window::GetSize().y >> 1 });
	m_camera.Invert(false, true);
	s_instance = this;

	// Initialize Event Handlers
	m_event_handler.Register(&DemoApplication::OnMousePress, this);
	m_event_handler.Register(&DemoApplication::OnMouseRelease, this);
	m_event_handler.Register(&DemoApplication::OnKeyPress, this);
	m_event_handler.Register(&DemoApplication::OnKeyRelease, this);
	m_event_handler.Register(&DemoApplication::OnWindowResize, this);

	// Initialize UI
	CreateSimpleUI();
	UI::Manager::Bind(action_map);
	UI::Manager::Bind("main_menu");
	glm::ivec2 resolution = Window::GetSize();
	UI::Layer::Resize(resolution.x, resolution.y);

	action_map.RegisterAction(ActionMap::Source::KEYBOARD, KEY_ESCAPE, true, false, [](bool start) {
		s_display_console = !s_display_console;
		});

	action_map.RegisterAction(ActionMap::Source::KEYBOARD, KEY_F8, true, false, [](bool start) {
		s_instance->ReloadShaders();
		});

	Logger::Log(net::Address::Local(), net::Address::Public());
}

void DemoApplication::ReloadShaders() {
	m_panel_shader.Compile();
	m_text_shader.Compile();
}

void DemoApplication::Run() {
	RenderEngine::SetClearColour(1.f, 0.f, 0.f, 1.f);
	RenderEngine::SetClearMode({ BufferType::COLOUR, BufferType::DEPTH });
	RenderEngine::SetTopology(MeshTopology::TRIANGLE_STRIP);
	RenderEngine::SetDepthTest(DepthTest::LESS);

	m_panel_shader.SetSource("default_ui_panel.glsl").Compile();
	m_text_shader.SetSource("default_ui_text.glsl").Compile();
	VertexLayout layout(m_panel_shader, {}, { { "quad", VertexDataType::INT_32, 4} });
	layout.Bind();

	// Initialize Font
	UI::Font& font = UI::Font::Create("assets/fonts/ZoreFont.zbt", Texture::Format::R8U);
	Texture::SetNamedTextureSlot("font", 1);
	font.GetTextureArray().Bind("font");
	TextureSampler sampler;
	sampler.SetFilters(TextureSampler::Filter::NEAREST, TextureSampler::Filter::NEAREST);
	sampler.Bind(1);

	std::unordered_map <int, Shader*> ui_shaders;
	ui_shaders[static_cast<int>(UI::Element::Type::PANEL)] = &m_panel_shader;
	ui_shaders[static_cast<int>(UI::Element::Type::LABEL)] = &m_text_shader;

	while (!Window::ShouldClose() && !s_quit) {
		RenderEngine::Clear();

		for (const UI::DrawCommand& command : UI::Layer::GetDrawList()) {
			ui_shaders[static_cast<int>(command.type)]->SetFloat4("camera", { m_camera.GetPosition(), m_camera.GetScale() });
			command.buffer->Bind();
			RenderEngine::DrawLinearInstanced(4, command.count);
		}

		Editor::BeginFrame();
		if (s_display_console)
			Console::Draw();
			//Editor::ShowDemoWindow();
		Editor::EndFrame();
		Window::Update();
	}
}

void DemoApplication::CreateSimpleUI() {
	using namespace UI::literals;

	// Define Styles
	UI::Style& standard_menu_style = UI::Style::Create("standard_menu")
		.SetMaxHeight(500_px)
		.SetMargin(0_px, AUTO());

	float perc = 100.f / 18.f;
	UI::Style& standard_button_style = UI::Style::Create("standard_button")
		.SetHeight(UI::Unit::PC(perc * 4), 4.f)
		.SetMargin(AUTO(), UI::Unit::PC(perc))
		.SetPadding(3_ph, 6_ph, 3_ph, 3_ph)
		.SetColour(0xFFFFFFFF);

	UI::Style& standard_text_style = UI::Style::Create("standard_text")
		.SetHeight(100_ph)
		.SetMargin(AUTO(), 0_px)
		.SetColour(0xAAAAAAFF);

	// Main Menu Demo UI
	UI::Layer& main_menu = UI::Manager::CreateLayer("main_menu");
	UI::Element& button_container = main_menu.AddChild(UI::Element::Type::PANEL, "standard_menu");
	UI::Element& play_button = button_container.AddChild(UI::Element::Type::BUTTON, "standard_button");
	UI::Element& options_button = button_container.AddChild(UI::Element::Type::BUTTON, "standard_button");
	UI::Element& quit_button = button_container.AddChild(UI::Element::Type::BUTTON, "standard_button");

	play_button.AddChild(UI::Element::Type::LABEL, "standard_text").SetText("PLAY");
	options_button.AddChild(UI::Element::Type::LABEL, "standard_text").SetText("OPTIONS");
	quit_button.AddChild(UI::Element::Type::LABEL, "standard_text").SetText("QUIT");

	action_map.RegisterAction(ActionMap::Source::INTERNAL, quit_button.GetUUID(), true, false, [](bool start) {
		UI::Manager::Bind("confirm_quit_menu");
		});

	// Sub Menu Demo UI
	UI::Layer& confirm_quit_menu = UI::Manager::CreateLayer("confirm_quit_menu");
	UI::Element& confirm_container = confirm_quit_menu.AddChild(UI::Element::Type::PANEL, "standard_menu");
	UI::Element& yes_button = confirm_container.AddChild(UI::Element::Type::BUTTON, "standard_button");
	UI::Element& no_button = confirm_container.AddChild(UI::Element::Type::BUTTON, "standard_button");

	yes_button.AddChild(UI::Element::Type::LABEL, "standard_text").SetText("YES");
	no_button.AddChild(UI::Element::Type::LABEL, "standard_text").SetText("NO");

	action_map.RegisterAction(ActionMap::Source::INTERNAL, yes_button.GetUUID(), false, true, [](bool start) {
		s_quit = true;
		});

	action_map.RegisterAction(ActionMap::Source::INTERNAL, no_button.GetUUID(), false, true, [](bool start) {
		UI::Manager::Bind("main_menu");
		});
}

DemoApplication* DemoApplication::Get() {
	return s_instance;
}

bool DemoApplication::OnMousePress(const MousePressedEvent& e) {
	return action_map.HandleEvent(ActionMap::Source::MOUSE, e.button, 1);
}

bool DemoApplication::OnMouseRelease(const MouseReleasedEvent& e) {
	return action_map.HandleEvent(ActionMap::Source::MOUSE, e.button, 0);
}

bool DemoApplication::OnKeyPress(const KeyPressedEvent& e) {
	return action_map.HandleEvent(ActionMap::Source::KEYBOARD, e.key, 1);
}

bool DemoApplication::OnKeyRelease(const KeyReleasedEvent& e) {
	return action_map.HandleEvent(ActionMap::Source::KEYBOARD, e.key, 0);
}

bool DemoApplication::OnWindowResize(const WindowResizedEvent& e) {
	m_camera.SetHeight(static_cast<float>(e.height));
	m_camera.SetAspectRatio(e.aspect_ratio);
	m_camera.SetPosition({ e.width >> 1, e.height >> 1 });
	return false;
}

Application* Application::Create() {
	return new DemoApplication({
		.enable_audio = false,
		.enable_networking = true,
		.enable_multi_viewports = true
		});
}