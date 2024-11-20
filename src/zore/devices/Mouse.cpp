#include "zore/devices/Mouse.hpp"
#include "zore/ui/Editor.hpp"
#include "zore/Debug.hpp"

#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Mouse Class
	//========================================================================

	static glm::vec2 s_position = { 0, 0 };
	static bool s_button_states[MOUSE_BUTTON_COUNT];

	void Mouse::ClearState(float x, float y) {
		memset(&s_button_states, 0, MOUSE_BUTTON_COUNT);
		s_position = { x, y };
	}

	const glm::vec2& Mouse::GetPosition() {
		return s_position;
	}

	bool Mouse::GetButton(int button) {
		return s_button_states[button];
	}

	//------------------------------------------------------------------------
	//	GLFW Mouse Callbacks
	//------------------------------------------------------------------------

	static std::vector<Mouse::Listener*> listeners;

	void Mouse::MoveCallback(GLFWwindow* windowHandle, double xpos, double ypos) {
		glm::vec2 delta = s_position;
		s_position = { xpos, ypos };
		delta = s_position - delta;

		if (Editor::WantsMouse())
			return;

		auto iter = listeners.begin();
		while (iter != listeners.end() && !(*iter)->OnMouseMove(s_position.x, s_position.y, delta.x, delta.y))
			iter++;
	}

	void Mouse::ButtonCallback(GLFWwindow* windowHandle, int button, int action, int mods) {
		if (Editor::WantsMouse())
			return;

		s_button_states[button] = action;
		auto iter = listeners.begin();
		if (action == GLFW_PRESS)
			while (iter != listeners.end() && !(*iter)->OnMousePress(button))
				iter++;
		else
			while (iter != listeners.end() && !(*iter)->OnMouseRelease(button))
				iter++;
	}

	void Mouse::ScrollCallback(GLFWwindow* windowHandle, double xOffset, double yOffset) {
		if (Editor::WantsMouse())
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