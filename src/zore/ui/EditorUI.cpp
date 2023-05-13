#include "zore/ui/EditorUI.hpp"
#include "zore/devices/Window.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glfw/glfw3.h>

namespace zore {

	static ImGuiIO* io = nullptr;

	void EditorUI::Init(bool dynamicViewports) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO();

		io->IniFilename = "config/imgui.cfg";

		io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		if (dynamicViewports)
			io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		io->ConfigDockingWithShift = true;
		//io->ConfigViewportsNoAutoMerge = true;
		io->ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.f;
		style.WindowRounding = 0.f;
		style.Colors[ImGuiCol_WindowBg].w = 1.f;

		ImGui_ImplGlfw_InitForOpenGL(Window::GetWindowHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void EditorUI::Cleanup() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorUI::BeginFrame() {
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void EditorUI::EndFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (DynamicViewportsEnabled()) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	bool EditorUI::WantsMouse() {
		return io->WantCaptureMouse;
	}

	bool EditorUI::WantsKeyboard() {
		return io->WantCaptureKeyboard;
	}

	bool EditorUI::DynamicViewportsEnabled() {
		return io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
	}
}