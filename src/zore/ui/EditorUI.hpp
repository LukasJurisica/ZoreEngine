#pragma once

namespace zore {

	class EditorUI {
	public:
		static void Init(bool dynamicViewports);
		static void Cleanup();

		static void BeginFrame();
		static void EndFrame();

		static bool WantsMouse();
		static bool WantsKeyboard();
		static bool DynamicViewportsEnabled();
	};
}