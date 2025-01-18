#include "main.hpp"
#include <zore/core/FileManager.hpp>
#include <zore/utils/ConfigManager.hpp>
#include <zore/audio/AudioEngine.hpp>
#include <zore/ui/Manager.hpp>
#include <zore/ui/Editor.hpp>
#include <zore/ui/Font.hpp>
#include <zore/Debug.hpp>

using namespace zore;

static bool s_display_console = false;
static DemoApplication* s_instance = nullptr;

DemoApplication::DemoApplication(const LaunchOptions& options) : Application(options), m_camera(Window::GetAspectRatio(), 4.f) {
	FileManager::Init("/examples/01_sandbox application/");
	RenderEngine::SetVSync(false);
	m_camera.SetHeight(static_cast<float>(Window::GetSize().y));
	m_camera.SetPosition({ Window::GetSize().x >> 1, Window::GetSize().y >> 1 });
	s_instance = this;

	// Initialize UI
	UI::Manager::Bind(action_map);
	UI::Manager::Bind("main_menu");

	Logger::Log(sizeof(UI::Style));

	action_map.RegisterAction(ActionMap::Source::KEYBOARD, KEY_ESCAPE, true, false, [](bool start) {
		s_display_console = !s_display_console;
		});

	action_map.RegisterAction(ActionMap::Source::KEYBOARD, KEY_F8, true, false, [](bool start) {
		s_instance->ReloadShaders();
		});
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

	CreateSimpleUI();

	// Initialize Font
	UI::Font& font = UI::Font::Create("assets/fonts/ZoreFont.zbt", Texture::Format::R8U);
	Texture::SetNamedTextureSlot("font", 1);
	font.GetTextureArray().Bind("font");
	TextureSampler sampler;
	sampler.SetFilters(TextureSampler::Filter::NEAREST, TextureSampler::Filter::NEAREST);
	sampler.Bind(1);

	while (!Window::ShouldClose()) {
		RenderEngine::Clear();

		m_panel_shader.SetFloat4("camera", { m_camera.GetPosition(), m_camera.GetScale() });
		for (const UI::DrawCommand& command : UI::Layer::GetDrawList()) {
			command.buffer->Bind();
			RenderEngine::DrawLinearInstanced(4, command.count);
		}

		m_text_shader.SetFloat4("camera", { m_camera.GetPosition(), m_camera.GetScale() });
		RenderEngine::DrawLinearInstanced(4, 1);

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
		.SetPadding(3_ph)
		.SetColour(0xFFFFFFFF);

	UI::Style& standard_text_style = UI::Style::Create("standard_text")
		.SetHeight(100_ph)
		.SetMargin(AUTO(), 0_px)
		.SetColour(0xAA00AAFF);

	// Main Menu Demo UI
	UI::Layer& main_menu = UI::Manager::CreateLayer("main_menu");
	UI::Element& button_container = main_menu.AddChild(UI::Element::Type::PANEL, "standard_menu");
	UI::Element& play_button = button_container.AddChild(UI::Element::Type::BUTTON, "standard_button");
	UI::Element& options_button = button_container.AddChild(UI::Element::Type::BUTTON, "standard_button");
	UI::Element& quit_button = button_container.AddChild(UI::Element::Type::BUTTON, "standard_button");

	play_button.AddChild(UI::Element::Type::LABEL, "standard_text").SetText("Play");
	options_button.AddChild(UI::Element::Type::LABEL, "standard_text").SetText("Options");
	quit_button.AddChild(UI::Element::Type::LABEL, "standard_text").SetText("Quit");

	action_map.RegisterAction(ActionMap::Source::INTERNAL, quit_button.GetUUID(), true, false, [](bool start) {
		UI::Manager::Bind("confirm_quit_menu");
		});

	glm::ivec2 resolution = Window::GetSize();
	UI::Layer::Resize(resolution.x, resolution.y);

	// Sub Menu Demo UI
	UI::Layer& confirm_quit_menu = UI::Manager::CreateLayer("confirm_quit_menu");
	UI::Element& confirm_container = confirm_quit_menu.AddChild(UI::Element::Type::PANEL, "standard_menu");
	UI::Element& yes = confirm_container.AddChild(UI::Element::Type::BUTTON, "standard_button");
	UI::Element& no = confirm_container.AddChild(UI::Element::Type::BUTTON, "standard_button");
	action_map.RegisterAction(ActionMap::Source::INTERNAL, no.GetUUID(), false, true, [](bool start) {
		UI::Manager::Bind("main_menu");
		});
}

DemoApplication* DemoApplication::Get() {
	return s_instance;
}

bool DemoApplication::OnMouseMove(float x, float y, float dx, float dy) {
	return false;
}

bool DemoApplication::OnMousePress(int button) {
	return action_map.HandleEvent(ActionMap::Source::MOUSE, button, 1);
}

bool DemoApplication::OnMouseRelease(int button) {
	return action_map.HandleEvent(ActionMap::Source::MOUSE, button, 0);
}

bool DemoApplication::OnKeyPress(int key, int mods) {
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
		.enable_audio = false,
		.enable_multi_viewports = true
		});
}