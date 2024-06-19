#include "zore/devices/Keyboard.hpp"
#include "zore/ui/EditorUI.hpp"
#include "zore/Debug.hpp"

#include <glfw/glfw3.h>
#include <bitset>
#include <vector>

namespace zore {

	//========================================================================
	//	Keyboard class
	//========================================================================

	static std::bitset<KEY_COUNT> s_key_held_states;
	static std::bitset<KEY_COUNT> s_key_down_states;
	static std::bitset<KEY_COUNT> s_key_up_states;

	bool Keyboard::GetKey(int key) {
		DEBUG_ENSURE(key < KEY_COUNT, "Attempted to access a key outside of the allowed range.");
		return s_key_held_states[key];
	}

	bool Keyboard::GetKeyUp(int key) {
		DEBUG_ENSURE(key < KEY_COUNT, "Attempted to access a key outside of the allowed range.");
		return s_key_up_states[key];
	}

	bool Keyboard::GetKeyDown(int key) {
		DEBUG_ENSURE(key < KEY_COUNT, "Attempted to access a key outside of the allowed range.");
		return s_key_down_states[key];
	}

	bool Keyboard::GetKeyMod(KeyMod keymod) {
		switch (keymod) {
		case zore::Keyboard::KeyMod::SHIFT:
			return s_key_held_states[KEY_L_SHIFT] || s_key_held_states[KEY_R_SHIFT];
		case zore::Keyboard::KeyMod::CTRL:
			return s_key_held_states[KEY_L_CTRL] || s_key_held_states[KEY_R_CTRL];
		case zore::Keyboard::KeyMod::ALT:
			return s_key_held_states[KEY_L_ALT] || s_key_held_states[KEY_R_ALT];
		}
		return false;
	}

	bool Keyboard::GetKeyMod(int mods, KeyMod keymod) {
		return mods & static_cast<int>(keymod);
	}

	void Keyboard::ClearState(bool clear_held_state) {
		if (clear_held_state)
			s_key_held_states.reset();
		s_key_down_states.reset();
		s_key_up_states.reset();
	}

	//------------------------------------------------------------------------
	//	GLFW Keyboard Callbacks
	//------------------------------------------------------------------------

	static std::vector<Keyboard::Listener*> listeners;

	void Keyboard::KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
		if (EditorUI::WantsKeyboard())
			return;

		if (action != KEY_REPEAT && key > 0 && key < KEY_COUNT) {
			s_key_held_states[key] = action;

			auto iter = listeners.begin();
			if (action == GLFW_PRESS) {
				s_key_down_states[key] = 1;
				while (iter != listeners.end() && !(*iter)->OnKeyPress(key, mods))
					iter++;
			}
			else {
				s_key_up_states[key] = 1;
				while (iter != listeners.end() && !(*iter)->OnKeyRelease(key))
					iter++;
			}
		}
	}

	void Keyboard::CharCallback(GLFWwindow* windowHandle, unsigned int code) {
		if (EditorUI::WantsKeyboard())
			return; 
		
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