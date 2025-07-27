#include "zore/devices/Mouse.hpp"
#include "zore/devices/Window.hpp"
#include "zore/events/MouseEvents.hpp"
#include "zore/events/Manager.hpp"
#include "zore/ui/Editor.hpp"
#include "zore/Debug.hpp"

#include <GLFW/glfw3.h>
#include <bitset>

namespace zore {

	//========================================================================
	//	Mouse Class
	//========================================================================

	static glm::vec2 s_position = { 0, 0 };

	static std::bitset<MOUSE_BUTTON_COUNT> s_button_held_states;
	static std::bitset<MOUSE_BUTTON_COUNT> s_button_up_states;
	static std::bitset<MOUSE_BUTTON_COUNT> s_button_down_states;

	bool Mouse::GetButton(int button) {
		DEBUG_ENSURE(button >= 0 && button < MOUSE_BUTTON_COUNT, "Attempted to access a button outside of the allowed range.");
		return s_button_held_states[button];
	}

	bool Mouse::GetButtonUp(int button) {
		DEBUG_ENSURE(button >= 0 && button < MOUSE_BUTTON_COUNT, "Attempted to access a button outside of the allowed range.");
		return s_button_up_states[button];
	}

	bool Mouse::GetButtonDown(int button) {
		DEBUG_ENSURE(button >= 0 && button < MOUSE_BUTTON_COUNT, "Attempted to access a button outside of the allowed range.");
		return s_button_down_states[button];
	}

	const glm::vec2& Mouse::GetPosition() {
		return s_position;
	}

	void Mouse::SetPosition(float x, float y) {
		s_position = { x, y };
		glfwSetCursorPos(Window::GetWindowHandle(), x, y);
	}

	void Mouse::ClearState(bool clear_held_state) {
		if (clear_held_state)
			s_button_held_states.reset();
		s_button_up_states.reset();
		s_button_down_states.reset();
	}

	//------------------------------------------------------------------------
	//	GLFW Mouse Callbacks
	//------------------------------------------------------------------------

	void Mouse::ButtonCallback(GLFWwindow* windowHandle, int button, int action, int mods) {
		if (Editor::WantsMouse())
			return;
		s_button_held_states[button] = action;
		if (action == GLFW_PRESS) {
			s_button_down_states[button] = true;
			event::Manager::Dispatch(MousePressedEvent(button, mods));
		}
		else {
			s_button_up_states[button] = true;
			event::Manager::Dispatch(MouseReleasedEvent(button));
		}
	}

	void Mouse::ScrollCallback(GLFWwindow* windowHandle, double xOffset, double yOffset) {
		if (Editor::WantsMouse())
			return;
		event::Manager::Dispatch(MouseScrolledEvent(static_cast<float>(xOffset), static_cast<float>(yOffset)));
	}

	void Mouse::MoveCallback(GLFWwindow* windowHandle, double xpos, double ypos) {
		glm::vec2 delta = s_position;
		s_position = { xpos, ypos };
		delta = s_position - delta;
		if (Editor::WantsMouse())
			return;
		event::Manager::Dispatch(MouseMovedEvent(s_position.x, s_position.y, delta.x, delta.y));
	}

	void Mouse::EnterCallback(GLFWwindow* window_handle, int entered) {
		event::Manager::Dispatch(MouseEnteredEvent(entered));
	}
}