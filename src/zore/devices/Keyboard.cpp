#include "zore/devices/keyboard.hpp"
#include "zore/events/keyboard_events.hpp"
#include "zore/events/manager.hpp"
#include "zore/ui/editor.hpp"
#include "zore/debug.hpp"
#include <GLFW/glfw3.h>
#include <bitset>

namespace zore {

	//========================================================================
	//	Keyboard class
	//========================================================================

	static std::bitset<KEY_COUNT> s_key_held_states;
	static std::bitset<KEY_COUNT> s_key_up_states;
	static std::bitset<KEY_COUNT> s_key_down_states;

	bool Keyboard::GetKey(int key) {
		DEBUG_ENSURE(key >= 0 && key < KEY_COUNT, "Attempted to access a key outside of the allowed range.");
		return s_key_held_states[key];
	}

	bool Keyboard::GetKeyUp(int key) {
		DEBUG_ENSURE(key >= 0 && key < KEY_COUNT, "Attempted to access a key outside of the allowed range.");
		return s_key_up_states[key];
	}

	bool Keyboard::GetKeyDown(int key) {
		DEBUG_ENSURE(key >= 0 && key < KEY_COUNT, "Attempted to access a key outside of the allowed range.");
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
		s_key_up_states.reset();
		s_key_down_states.reset();
	}

	//------------------------------------------------------------------------
	//	GLFW Keyboard Callbacks
	//------------------------------------------------------------------------

	void Keyboard::KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
		if (Editor::WantsKeyboard())
			return;

		if (action != KEY_REPEAT && key > 0 && key < KEY_COUNT) {
			s_key_held_states[key] = action;

			if (action == GLFW_PRESS) {
				s_key_down_states[key] = true;
				event::Manager::Dispatch(KeyPressedEvent(key, mods));
			}
			else {
				s_key_up_states[key] = true;
				event::Manager::Dispatch(KeyReleasedEvent(key));
			}
		}
	}

	void Keyboard::CharCallback(GLFWwindow* windowHandle, unsigned int code) {
		if (Editor::WantsKeyboard())
			return; 
	}
}