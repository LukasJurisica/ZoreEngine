#include "devices/Keyboard.hpp"
#include "devices/Window.hpp"
#include "debug/debug.hpp"
#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Keyboard class
	//========================================================================

	std::bitset<KEY_COUNT> keyStates;

	void Keyboard::ClearState() {
		keyStates.reset();
	}

	bool Keyboard::GetKey(unsigned char key) {
		return keyStates[key];
	}

	//------------------------------------------------------------------------
	//	GLFW Keyboard Callbacks
	//------------------------------------------------------------------------

	std::vector<KeyListener*> listeners;

	void Keyboard::KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
		if (action != GLFW_REPEAT) {
			if (key > 0 && key < KEY_COUNT)
				keyStates[key] = action;
			else
				Logger::Warn("Key Pressed out of range: " + std::to_string(key));

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

	//========================================================================
	//	Keyboard Listener Class
	//========================================================================

	KeyListener::KeyListener() {
		listeners.push_back(this);
	}

	KeyListener::~KeyListener() {
		auto iter = find(listeners.begin(), listeners.end(), this);
		listeners.erase(iter);
	}
}