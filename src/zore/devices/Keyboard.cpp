#include "zore/devices/Keyboard.hpp"
#include "zore/ui/EditorUI.hpp"
#include "zore/debug/debug.hpp"
#include <glfw/glfw3.h>
#include <algorithm>

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
		}
		return false;
	}

	bool Keyboard::GetKeyMod(int mods, KeyMod keymod) {
		return mods & static_cast<int>(keymod);
	}

	//------------------------------------------------------------------------
	//	GLFW Keyboard Callbacks
	//------------------------------------------------------------------------

	static std::vector<Keyboard::Listener*> listeners;

	void Keyboard::KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
		if (action != KEY_REPEAT) {
			if (key > 0 && key < KEY_LAST)
				keyStates[key] = action;

			auto iter = listeners.begin();
			if (action == GLFW_PRESS)
				while (iter != listeners.end() && !(*iter)->OnKeyPress(key, mods))
					iter++;
			else
				while (iter != listeners.end() && !(*iter)->OnKeyRelease(key))
					iter++;
		}
	}

	void Keyboard::CharCallback(GLFWwindow* windowHandle, unsigned int code) {
		auto iter = listeners.begin();
		while (iter != listeners.end() && !(*iter)->OnChar(code))
			iter++;
	}

	//========================================================================
	//	Keyboard Listener Class
	//========================================================================

	Keyboard::Listener::Listener(int priority) : m_priority(priority) {
		auto iter = listeners.begin();
		while (iter != listeners.end() && (*iter)->m_priority > m_priority)
			iter++;
		listeners.insert(iter, this);
	}

	Keyboard::Listener::~Listener() {
		listeners.erase(std::find(listeners.begin(), listeners.end(), this));
	}
}