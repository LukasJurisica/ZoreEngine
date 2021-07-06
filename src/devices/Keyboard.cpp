#include "devices/Keyboard.hpp"
#include "devices/Window.hpp"
#include "debug/debug.hpp"
#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Keyboard class
	//========================================================================

	Keyboard::Keyboard(const std::string& name) {
		KeyListener::ClaimWaiters(this, name);
	}

	Keyboard::~Keyboard() {
		KeyListener::ReleaseListeners(listeners);
	}

	void Keyboard::RegisterListener(KeyListener* listener) {
		listeners.push_back(listener);
	}

	void Keyboard::UnregisterListener(KeyListener* listener) {
		auto iter = std::find(listeners.begin(), listeners.end(), listener);
		if (iter != listeners.end())
			listeners.erase(iter);
	}

	void Keyboard::ClearState() {
		keyStates.reset();
	}

	bool Keyboard::GetKey(unsigned char key) const {
		return keyStates[key];
	}

	//---------------------------------------
	//	Keyboard Callbacks and Event Handling
	//---------------------------------------

	void Keyboard::OnKeyPress(unsigned char key, int mods, bool repeat) {
		if (!repeat) {
			keyStates[key] = true;
			for (KeyListener* listener : listeners)
				listener->OnKeyPress(key, mods);
		}
	}

	void Keyboard::OnKeyRelease(unsigned char key) {
		keyStates[key] = false;
		for (KeyListener* listener : listeners)
			listener->OnKeyRelease(key);
	}

	void Keyboard::OnChar(char c) {
		for (KeyListener* listener : listeners)
			listener->OnChar(c);
	}

	//------------------------------------------------------------------------
	//	GLFW Keyboard Callbacks
	//------------------------------------------------------------------------

	void Keyboard::KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));

		if (action > 0)
			window->GetKeyboard()->OnKeyPress(key, mods, action - 1);
		else
			window->GetKeyboard()->OnKeyRelease(key);
	}

	void Keyboard::CharCallback(GLFWwindow* windowHandle, unsigned int code) {
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
		window->GetKeyboard()->OnChar(code);
	}

	//========================================================================
	//	Keyboard Listener Class
	//========================================================================

	std::vector<KeyListener*> waiters;

	KeyListener::KeyListener(const std::string& name) : name(name), keyboard(nullptr) {
		Window* window = Window::Get(name);
		if (window) {
			keyboard = window->GetKeyboard();
			keyboard->RegisterListener(this);
		}
		else
			waiters.push_back(this);
	}

	KeyListener::~KeyListener() {
		if (keyboard)
			keyboard->UnregisterListener(this);
		else 
			RemoveWaiter(this);
	}

	void KeyListener::RemoveWaiter(KeyListener* listener) {
		for (int i = 0; i < waiters.size();) {
			if (waiters[i] == listener)
				waiters.erase(waiters.begin() + i);
			else
				i++;
		}
	}

	void KeyListener::ClaimWaiters(Keyboard* keyboard, const std::string& name) {
		for (int i = 0; i < waiters.size();) {
			if (waiters[i]->name == name || waiters[i]->name == "") {
				keyboard->RegisterListener(waiters[i]);
				waiters[i]->keyboard = keyboard;
				waiters.erase(waiters.begin() + i);
			}
			else
				i++;
		}
	}

	void KeyListener::ReleaseListeners(std::vector<KeyListener*>& listeners) {
		for (KeyListener* listener : listeners) {
			listener->keyboard = nullptr;
			waiters.push_back(listener);
		}
	}
}