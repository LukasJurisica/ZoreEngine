#pragma once
#include "glm/ext/vector_int2.hpp"

struct GLFWwindow;
struct GLFWMonitor;

namespace zore {

	//========================================================================
	//	Window Class
	//========================================================================

	class Window {
		friend class Application;
	public:
		static void Update();
		static bool ShouldClose();
		static void SetTitle(const char* title);
		static void SetResizable(bool value);
		static void SetMaximized(bool value);
		static void SetBorderless(bool value);
		static void SetFullscreen(bool value);
		static void ToggleFullscreen();
		static void BringToFront();
		static void HideCursor(bool value);
		static void ToggleCursor();
		static void SetSize(int width, int height);
		static void SetPosition(int x, int y);
		static void Centre();

		static bool GetMaximized();
		static float GetAspectRatio();		
		static const glm::ivec2& GetSize();
		static const glm::ivec2& GetPosition();
		static GLFWwindow* GetWindowHandle();
		static void GetNativeHandle(void* result);
		static glm::ivec2 GetNativeResolution(int monitorIndex = 0);

	private:
		static void Init(bool transparent);
		static void Create();
		static void Cleanup();

		static void ErrorCallback(int error, const char* description);
		static void ResizeCallback(GLFWwindow* windowHandle, int width, int height);
		static void MoveCallback(GLFWwindow* windowHandle, int xpos, int ypos);
		static void FocusCallback(GLFWwindow* windowHandle, int focused);

		//========================================================================
		//	Window Listener Class
		//========================================================================

	public:
		class Listener {
			friend class Window;
		protected:
			Listener();
			~Listener();

		private:
			virtual void OnWindowResize(int width, int height, float aspectRatio) {};
			virtual void OnWindowMove(int xpos, int ypos) {};
			virtual void OnWindowFocus(int focused) {};
		};
	};
}