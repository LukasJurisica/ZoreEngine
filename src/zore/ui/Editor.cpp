#include "zore/ui/Editor.hpp"
#include "zore/devices/Window.hpp"
#include "zore/core/FileManager.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glfw/glfw3.h>

namespace zore {

	static ImGuiIO* io = nullptr;

	void Editor::Init(bool enable_multi_viewports, bool enable_docking, bool enable_keyboard_navigation) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO();

		FileManager::EnsureDir("config");
		io->IniFilename = "config/imgui.cfg";
		if (enable_keyboard_navigation) // Enable Keyboard Controls
			io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		if (enable_docking) { // Enable Docking
			io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io->ConfigDockingWithShift = true;
		}
		if (enable_multi_viewports) { // Enable Multi-Viewport / Platform Windows
			io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			//io->ConfigViewportsNoAutoMerge = true;
			io->ConfigViewportsNoTaskBarIcon = true;
		}

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.f;
		style.WindowRounding = 0.f;
		style.Colors[ImGuiCol_WindowBg].w = 1.f;

		ImGui_ImplGlfw_InitForOpenGL(Window::GetWindowHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void Editor::Cleanup() {
		if (io) {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	}

	void Editor::ShowDemoWindow() {
		ImGui::ShowDemoWindow();
	}

	void Editor::BeginFrame() {
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void Editor::EndFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (DynamicViewportsEnabled()) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	bool Editor::WantsMouse() {
		return io && io->WantCaptureMouse;
	}

	bool Editor::WantsKeyboard() {
		return io && io->WantCaptureKeyboard;
	}

	bool Editor::DynamicViewportsEnabled() {
		return io && (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable);
	}
}