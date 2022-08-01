#pragma once
#include "glm/ext/vector_int2.hpp"

struct GLFWwindow;

namespace zore {

	//========================================================================
	//	Window Class
	//========================================================================

	class Window {
	public:
		static void Init(int width, int height);
		static void Cleanup();

		static void Update();
		static bool ShouldClose();
		static void SetTitle(const char* title);
		static void SetBorderless(bool value);
		static void SetFullscreen(bool value);
		static void ToggleFullscreen();
		static void HideCursor(bool value);
		static void ToggleCursor();
		static void SetSize(int width, int height);
		static void SetPosition(int x, int y);
		static void Centre();

		static float GetAspectRatio();
		static const glm::ivec2& GetSize();
		static const glm::ivec2 GetNativeResolution();

	private:
		static void ErrorCallback(int error, const char* description);
		static void ResizeCallback(GLFWwindow* windowHandle, int width, int height);
		static void MoveCallback(GLFWwindow* windowHandle, int xpos, int ypos);
		static void FocusCallback(GLFWwindow* windowHandle, int focused);
	};

	//========================================================================
	//	Window Listener Class
	//========================================================================

	class WindowListener {
		friend class Window;
	private:
		virtual void OnWindowResize(int width, int height) {};
		virtual void OnWindowMove(int xpos, int ypos) {};
		virtual void OnWindowFocus(int focused) {};

	protected:
		WindowListener();
		~WindowListener();
	};
}