#include "zore/devices/Keyboard.hpp"
#include "zore/ui/EditorUI.hpp"
#include "zore/debug/debug.hpp"
#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Keyboard class
	//========================================================================

	static std::bitset<KEY_LAST> keyStates;

	void Keyboard::ClearState() {
		keyStates.reset();
	}

	bool Keyboard::GetKey(int key) {
		DEBUG_ENSURE(key < KEY_LAST, "Attempted to access a key outside of the allowed range.");
		return keyStates[key];
	}

	bool Keyboard::GetKeyMod(KeyMod keymod) {
		switch (keymod) {
		case zore::Keyboard::KeyMod::CTRL:
			return keyStates[KEY_L_CTRL] || keyStates[KEY_R_CTRL];
		case zore::Keyboard::KeyMod::ALT:
			return keyStates[KEY_L_ALT] || keyStates[KEY_R_ALT];
		case zore::Keyboard::KeyMod::SHIFT:
			return keyStates[KEY_L_SHIFT] || keyStates[KEY_R_SHIFT];
		default:
			return false;
		}
	}

	//========================================================================
	//	Keyboard Listener Class
	//========================================================================

	static std::vector<KeyListener*> listeners;

	KeyListener::KeyListener() {
		listeners.push_back(this);
	}

	KeyListener::~KeyListener() {
		auto iter = find(listeners.begin(), listeners.end(), this);
		listeners.erase(iter);
	}

	//------------------------------------------------------------------------
	//	GLFW Keyboard Callbacks
	//------------------------------------------------------------------------

	void Keyboard::KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
		if (action != KEY_REPEAT) {
			if (key > 0 && key < KEY_LAST)
				keyStates[key] = action;

			if (action == GLFW_PRESS)
				for (KeyListener* listener : listeners)
					listener->OnKeyPress(key, mods);
			else
				for (KeyListener* listener : listeners)
					listener->OnKeyRelease(key);
		}
	}

	void Keyboard::CharCallback(GLFWwindow* windowHandle, unsigned int code) {
		for (KeyListener* listener : listeners)
			listener->OnChar(code);
	}
}