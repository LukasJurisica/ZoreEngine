#pragma once
#include "zore/devices/mouse_codes.hpp"
#include "zore/math/vector/vec2.hpp"

struct GLFWwindow;

namespace zore {

	//========================================================================
	//	Mouse
	//========================================================================

	class Mouse {
		friend class Window;
	public:
		static bool GetButton(int button);
		static bool GetButtonUp(int button);
		static bool GetButtonDown(int button);
		static const zm::vec2& GetPosition();
		static void SetPosition(float x = 0, float y = 0);
		static void SetPosition(const zm::vec2& position);
		static void ClearState(bool clear_held_state = true);

	private:
		static void ButtonCallback(GLFWwindow* window_handle, int button, int action, int mods);
		static void ScrollCallback(GLFWwindow* window_handle, double xoffset, double yoffset);
		static void MoveCallback(GLFWwindow* window_handle, double xpos, double ypos);
		static void EnterCallback(GLFWwindow* window_handle, int entered);
	};
}