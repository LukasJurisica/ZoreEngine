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

	class MouseListener;
	class Mouse {
		friend class Window;
	public:
		Mouse(const std::string& name);
		~Mouse();
		void RegisterListener(MouseListener* listener);
		void UnregisterListener(MouseListener* listener);

		void ClearState(float x = 0, float y = 0);
		const glm::vec2& GetPos() const;
		bool GetButton(int button) const;

	private:
		static void MoveCallback(GLFWwindow* windowHandle, double xpos, double ypos);
		static void ActionCallback(GLFWwindow* windowHandle, int button, int action, int mods);
		static void ScrollCallback(GLFWwindow* windowHandle, double xoffset, double yoffset);

		void OnMouseMove(float nx, float ny);
		void OnMousePress(int button);
		void OnMouseRelease(int button);
		void OnMouseScroll(float dx, float dy);

		glm::vec2 pos;
		bool buttonStates[MOUSE_BUTTON_COUNT];
		std::vector<MouseListener*> listeners;
	};

	//========================================================================
	//	Mouse Listener Class
	//========================================================================

	class MouseListener {
		friend class Mouse;
	private:
		static void RemoveWaiter(MouseListener* listener);
		static void ClaimWaiters(Mouse* mouse, const std::string& name);
		static void ReleaseListeners(std::vector<MouseListener*>& listeners);

		virtual void OnMouseMove(float nx, float ny, float dx, float dy) {};
		virtual void OnMousePress(int button) {};
		virtual void OnMouseRelease(int button) {};
		virtual void OnMouseScroll(float dx, float dy) {};

	protected:
		MouseListener(const std::string& name = "");
		~MouseListener();

		std::string name;
		Mouse* mouse;
	};
}