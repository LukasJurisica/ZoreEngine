#include "zore/ui/editor.hpp"
#include "zore/devices/window.hpp"
#include "zore/core/file_manager.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#if defined(ZORE_RENDERER_OPENGL)
#include <imgui_impl_opengl3.h>
#elif defined(ZORE_RENDERER_VULKAN)
#include <imgui_impl_vulkan.h>
#endif
#include <GLFW/glfw3.h>

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
		if (enable_docking) // Enable Docking
			io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		if (enable_multi_viewports) { // Enable Multi-Viewport / Platform Windows
			io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			io->ConfigViewportsNoTaskBarIcon = true;
		}

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.f;
		style.WindowRounding = 0.f;
		style.Colors[ImGuiCol_WindowBg].w = 1.f;

		
#if defined(ZORE_RENDERER_OPENGL)
		ImGui_ImplGlfw_InitForOpenGL(Window::GetWindowHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
#elif defined(ZORE_RENDERER_VULKAN)
		//ImGui_ImplGlfw_InitForVulkan(Window::GetWindowHandle(), true);
		//ImGui_ImplVulkan_InitInfo init_info = {};
		//ImGui_ImplVulkan_Init(&init_info);
#endif
	}

	ImGuiIO* Editor::GetIO() {
		return io;
	}

	void Editor::Cleanup() {
		if (io) {
#if defined(ZORE_RENDERER_OPENGL)
			ImGui_ImplOpenGL3_Shutdown();
#elif defined(ZORE_RENDERER_VULKAN)
			//ImGui_ImplVulkan_Shutdown();
#endif
			//ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	}

	void Editor::ShowDemoWindow() {
		ImGui::ShowDemoWindow();
	}

	void Editor::BeginFrame() {
		ImGui_ImplGlfw_NewFrame();
#if defined(ZORE_RENDERER_OPENGL)
		ImGui_ImplOpenGL3_NewFrame();
#elif defined(ZORE_RENDERER_VULKAN)
		ImGui_ImplVulkan_NewFrame();
#endif
		ImGui::NewFrame();
	}

	void Editor::EndFrame() {
		ImGui::Render();
#if defined(ZORE_RENDERER_OPENGL)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif defined(ZORE_RENDERER_VULKAN)
		//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData());
#endif

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