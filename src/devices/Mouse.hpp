#pragma once
#include "MouseCodes.hpp"
#include <vector>
#include <string>
#include <glm/ext/vector_float2.hpp>

struct GLFWwindow;
#define MOUSE_BUTTON_COUNT 5

namespace zore {

	//========================================================================
	//	Mouse Class
	//========================================================================

	class Mouse {
		friend class Window;
	public:
		static void ClearState(float x = 0, float y = 0);
		static const glm::vec2& GetPosition();
		static bool GetButton(int button);

	private:
		static void MoveCallback(GLFWwindow* windowHandle, double xpos, double ypos);
		static void ActionCallback(GLFWwindow* windowHandle, int button, int action, int mods);
		static void ScrollCallback(GLFWwindow* windowHandle, double xoffset, double yoffset);
	};

	//========================================================================
	//	Mouse Listener Class
	//========================================================================

	class MouseListener {
		friend class Mouse;
	protected:
		MouseListener();
		~MouseListener();

	private:
		virtual void OnMouseMove(float nx, float ny, float dx, float dy) {};
		virtual void OnMousePress(int button) {};
		virtual void OnMouseRelease(int button) {};
		virtual void OnMouseScroll(float dx, float dy) {};
	};
}