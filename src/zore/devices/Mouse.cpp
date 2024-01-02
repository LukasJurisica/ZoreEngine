#include "zore/devices/Mouse.hpp"
#include "zore/ui/EditorUI.hpp"
#include "zore/debug/debug.hpp"
#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Mouse Class
	//========================================================================

	static glm::vec2 position = { 0, 0 };
	static bool buttonStates[MOUSE_BUTTON_COUNT];

	void Mouse::ClearState(float x, float y) {
		memset(&buttonStates, 0, MOUSE_BUTTON_COUNT);
		position = { x, y };
	}

	const glm::vec2& Mouse::GetPosition() {
		return position;
	}

	bool Mouse::GetButton(int button) {
		return buttonStates[button];
	}

	//------------------------------------------------------------------------
	//	GLFW Mouse Callbacks
	//------------------------------------------------------------------------

	static std::vector<Mouse::Listener*> listeners;

	void Mouse::MoveCallback(GLFWwindow* windowHandle, double xpos, double ypos) {
		glm::vec2 oldPos = position;
		position = { xpos, ypos };

		if (EditorUI::WantsMouse())
			return;

		auto iter = listeners.begin();
		while (iter != listeners.end() && !(*iter)->OnMouseMove(position.x, position.y, position.x - oldPos.x, position.y - oldPos.y))
			iter++;
	}

	void Mouse::ButtonCallback(GLFWwindow* windowHandle, int button, int action, int mods) {
		buttonStates[button] = action;
		if (EditorUI::WantsMouse())
			return;

		auto iter = listeners.begin();
		if (action == GLFW_PRESS)
			while (iter != listeners.end() && !(*iter)->OnMousePress(button))
				iter++;
		else
			while (iter != listeners.end() && !(*iter)->OnMouseRelease(button))
				iter++;
	}

	void Mouse::ScrollCallback(GLFWwindow* windowHandle, double xOffset, double yOffset) {
		if (EditorUI::WantsMouse())
			return;

		auto iter = listeners.begin();
		while (iter != listeners.end() && !(*iter)->OnMouseScroll(static_cast<float>(xOffset), static_cast<float>(yOffset)))
			iter++;
	}

	//========================================================================
	//	Mouse Listener Class
	//========================================================================

	Mouse::Listener::Listener(int priority) : m_priority(priority) {
		auto iter = listeners.begin();
		while (iter != listeners.end() && (*iter)->m_priority > m_priority)
			iter++;
		listeners.insert(iter, this);
	}

	Mouse::Listener::~Listener() {
		listeners.erase(std::find(listeners.begin(), listeners.end(), this));
	}
}