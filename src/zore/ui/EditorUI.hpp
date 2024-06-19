#pragma once

namespace zore {

	struct EditorUIParams {
		bool multi_viewports = false;
		bool enable_docking = false;
		bool enable_keyboard_navigation = false;
	};

	class EditorUI {
	public:
		static void Init(const EditorUIParams& params);
		static void Cleanup();

		static void ShowDemoWindow();

		static void BeginFrame();
		static void EndFrame();

		static bool WantsMouse();
		static bool WantsKeyboard();
		static bool DynamicViewportsEnabled();
	};
}