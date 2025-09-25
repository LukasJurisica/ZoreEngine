#pragma once

struct ImGuiIO;

namespace zore {

	class Editor {
	public:
		static void Init(bool enable_multi_viewports, bool enable_docking, bool enable_keyboard_navigation);
		static ImGuiIO* GetIO();
		static void Cleanup();

		static void ShowDemoWindow();

		static void BeginFrame();
		static void EndFrame();

		static bool WantsMouse();
		static bool WantsKeyboard();
		static bool DynamicViewportsEnabled();
	};
}