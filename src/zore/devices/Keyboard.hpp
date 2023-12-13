#pragma once
#include "zore/devices/KeyCodes.hpp"
#include <bitset>
#include <vector>
#include <string>

struct GLFWwindow;

namespace zore {

	//========================================================================
	//	Keyboard Class
	//========================================================================

	class Keyboard {
		friend class Window;
	public:
		enum class KeyMod { SHIFT = 1, CTRL = 2, ALT = 4 };

	public:
		static bool GetKey(int key);
		static bool GetKeyMod(KeyMod keymod);
		static bool GetKeyMod(int mods, KeyMod keymod);
		static void ClearState();

	private:
		static void KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* windowHandle, unsigned int code);
	};

	//========================================================================
	//	Keyboard Listener Class
	//========================================================================

	class KeyListener {
		friend class Keyboard;
	protected:
		KeyListener();
		~KeyListener();

	private:
		virtual void OnKeyPress(int key, int mods) {};
		virtual void OnKeyRelease(int key) {};
		virtual void OnChar(char code) {};
	};
}