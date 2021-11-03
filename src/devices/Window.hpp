#pragma once
#include "glm/ext/vector_int2.hpp"

struct GLFWwindow;

namespace zore {

	//========================================================================
	//	Window Class
	//========================================================================

	class Window {
	public:
		Window(int width, int height);
		~Window();

		static void Init();
		static Window* Get();
		static void Cleanup();

		void Bind();
		void Update();
		bool ShouldClose();

		void SetTitle(const char* title);
		void SetFullscreen(bool value);
		void ToggleFullscreen();
		void SetBorderless(bool value);
		void SetSize(int width, int height);
		void HideCursor(bool value);
		void ToggleCursor();

		float GetAspectRatio();
		const glm::ivec2& GetSize();

	private:
		static void ErrorCallback(int error, const char* description);
		static void ResizeCallback(GLFWwindow* windowHandle, int width, int height);
		static void MoveCallback(GLFWwindow* windowHandle, int xpos, int ypos);
		static void FocusCallback(GLFWwindow* windowHandle, int focused);

		glm::ivec2 size;
		glm::ivec2 position;
		GLFWwindow* windowHandle;
		bool fullscreen;
		bool cursorHidden;
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