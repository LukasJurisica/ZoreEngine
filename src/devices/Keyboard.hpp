#pragma once
#include "KeyCodes.hpp"
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
		static bool GetKey(unsigned char key);
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
	private:
		virtual void OnKeyPress(unsigned char key, int mods) { };
		virtual void OnKeyRelease(unsigned char key) { };
		virtual void OnChar(char code) { };

	protected:
		KeyListener();
		~KeyListener();
	};
}