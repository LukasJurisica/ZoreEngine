#include "devices/Mouse.hpp"
#include "devices/Window.hpp"
#include "debug/debug.hpp"
#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Mouse Class
	//========================================================================

	Mouse::Mouse(const std::string& name) : buttonStates(), pos(0, 0) {
		MouseListener::ClaimWaiters(this, name);
	}

	Mouse::~Mouse() {
		MouseListener::ReleaseListeners(listeners);
	}

	void Mouse::RegisterListener(MouseListener* listener) {
		listeners.push_back(listener);
	}

	void Mouse::UnregisterListener(MouseListener* listener) {
		auto iter = std::find(listeners.begin(), listeners.end(), listener);
		if (iter != listeners.end())
			listeners.erase(iter);
	}

	void Mouse::ClearState(float x, float y) {
		memset(&buttonStates, 0, MOUSE_BUTTON_COUNT);
		pos = { x, y };
	}

	const glm::vec2& Mouse::GetPos() const {
		return pos;
	}

	bool Mouse::GetButton(int button) const {
		return buttonStates[button];
	}

	//---------------------------------------
	//	Mouse Callbacks and Event Handling
	//---------------------------------------

	void Mouse::OnMouseMove(float nx, float ny) {
		glm::vec2 oldPos = pos;
		pos = { nx, ny };
		for (MouseListener* listener : listeners)
			listener->OnMouseMove(nx, ny, nx - oldPos.x , ny - oldPos.y);
	}

	void Mouse::OnMousePress(int button) {
		buttonStates[button] = true;
		for (MouseListener* listener : listeners)
			listener->OnMousePress(button);
	}

	void Mouse::OnMouseRelease(int button) {
		buttonStates[button] = false;
		for (MouseListener* listener : listeners)
			listener->OnMouseRelease(button);
	}

	void Mouse::OnMouseScroll(float dx, float dy) {
		for (MouseListener* listener : listeners)
			listener->OnMouseScroll(dx, dy);
	}

	//------------------------------------------------------------------------
	//	GLFW Mouse Callbacks
	//------------------------------------------------------------------------

	void Mouse::MoveCallback(GLFWwindow* windowHandle, double xpos, double ypos) {
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
		window->GetMouse()->OnMouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
	}

	void Mouse::ActionCallback(GLFWwindow* windowHandle, int button, int action, int mods) {
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));

		if (action == GLFW_PRESS)
			window->GetMouse()->OnMousePress(button);
		else
			window->GetMouse()->OnMouseRelease(button);
	}

	void Mouse::ScrollCallback(GLFWwindow* windowHandle, double xOffset, double yOffset) {
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
		window->GetMouse()->OnMouseScroll(static_cast<float>(xOffset), static_cast<float>(yOffset));
	}

	//========================================================================
	//	Mouse Listener Class
	//========================================================================

	std::vector<MouseListener*> waiters;

	MouseListener::MouseListener(const std::string& name) : name(name), mouse(nullptr) {
		Window* window = Window::Get(name);
		if (window) {
			mouse = window->GetMouse();
			mouse->RegisterListener(this);
		}
		else
			waiters.push_back(this);
	}

	MouseListener::~MouseListener() {
		if (mouse)
			mouse->UnregisterListener(this);
		else
			RemoveWaiter(this);
	}

	void MouseListener::RemoveWaiter(MouseListener* listener) {
		for (int i = 0; i < waiters.size();) {
			if (waiters[i] == listener)
				waiters.erase(waiters.begin() + i);
			else
				i++;
		}
	}

	void MouseListener::ClaimWaiters(Mouse* mouse, const std::string& name) {
		for (int i = 0; i < waiters.size();) {
			if (waiters[i]->name == name || waiters[i]->name == "") {
				mouse->RegisterListener(waiters[i]);
				waiters[i]->mouse = mouse;
				waiters.erase(waiters.begin() + i);
			}
			else
				i++;
		}
	}

	void MouseListener::ReleaseListeners(std::vector<MouseListener*>& listeners) {
		for (MouseListener* listener : listeners) {
			listener->mouse = nullptr;
			waiters.push_back(listener);
		}
	}
}