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

	//========================================================================
	//	Mouse Listener Class
	//========================================================================

	static std::vector<MouseListener*> listeners;

	MouseListener::MouseListener() {
		listeners.push_back(this);
	}

	MouseListener::~MouseListener() {
		auto iter = find(listeners.begin(), listeners.end(), this);
		listeners.erase(iter);
	}

	//------------------------------------------------------------------------
	//	GLFW Mouse Callbacks
	//------------------------------------------------------------------------

	void Mouse::MoveCallback(GLFWwindow* windowHandle, double xpos, double ypos) {
		glm::vec2 oldPos = position;
		position = { xpos, ypos };

		if (EditorUI::WantsMouse())
			return;

		for (MouseListener* listener : listeners)
			listener->OnMouseMove(position.x, position.y, position.x - oldPos.x, position.y - oldPos.y);
	}

	void Mouse::ButtonCallback(GLFWwindow* windowHandle, int button, int action, int mods) {
		buttonStates[button] = action;
		if (EditorUI::WantsMouse())
			return;

		if (action == GLFW_PRESS)
			for (MouseListener* listener : listeners)
				listener->OnMousePress(button);
		else
			for (MouseListener* listener : listeners)
				listener->OnMouseRelease(button);
	}

	void Mouse::ScrollCallback(GLFWwindow* windowHandle, double xOffset, double yOffset) {
		if (EditorUI::WantsMouse())
			return;

		for (MouseListener* listener : listeners)
			listener->OnMouseScroll(static_cast<float>(xOffset), static_cast<float>(yOffset));
	}
}