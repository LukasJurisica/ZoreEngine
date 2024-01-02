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

		//========================================================================
		//	Keyboard Listener Class
		//========================================================================

	public:
		class Listener {
			friend class Keyboard;
		protected:
			Listener(int priority = 0);
			~Listener();

		private:
			virtual bool OnKeyPress(int key, int mods) { return false; };
			virtual bool OnKeyRelease(int key) { return false; };
			virtual bool OnChar(char code) { return false; };

		private:
			int m_priority;
		};
	};
}