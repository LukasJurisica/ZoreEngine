#pragma once
#include "zore/devices/MouseCodes.hpp"
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
		static void ButtonCallback(GLFWwindow* windowHandle, int button, int action, int mods);
		static void ScrollCallback(GLFWwindow* windowHandle, double xoffset, double yoffset);

		//========================================================================
		//	Mouse Listener Class
		//========================================================================

	public:
		class Listener {
			friend class Mouse;
		protected:
			Listener(int priority = 0);
			~Listener();

		private:
			virtual bool OnMouseMove(float nx, float ny, float dx, float dy) { return false; };
			virtual bool OnMousePress(int button) { return false; };
			virtual bool OnMouseRelease(int button) { return false; };
			virtual bool OnMouseScroll(float dx, float dy) { return false; };

		private:
			int m_priority;
		};
	};

}