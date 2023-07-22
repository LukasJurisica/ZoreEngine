#pragma once

namespace zore {

	class EditorUI {
	public:
		static void Init(bool multiViewports = false, bool enableDocking = false);
		static void Cleanup();

		static void ShowDemoWindow();

		static void BeginFrame();
		static void EndFrame();

		static bool WantsMouse();
		static bool WantsKeyboard();
		static bool DynamicViewportsEnabled();
	};
}