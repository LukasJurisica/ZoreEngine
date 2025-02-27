#pragma once
#include "zore/devices/KeyCodes.hpp"

struct GLFWwindow;

namespace zore {

	//========================================================================
	//	Keyboard Class
	//========================================================================

	class Keyboard {
		friend class Window;
	public:
		enum class KeyMod { SHIFT = (1 << 0), CTRL = (1 << 1), ALT = (1 << 2) };

	public:
		static bool GetKey(int key);
		static bool GetKeyUp(int key);
		static bool GetKeyDown(int key);
		static bool GetKeyMod(KeyMod keymod);
		static bool GetKeyMod(int mods, KeyMod keymod);
		static void ClearState(bool clear_held_state = true);

	private:
		static void KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* windowHandle, unsigned int code);
	};
}