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
		static void Close(bool value = true);
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
		static void SetMinSize(int width = -1, int height = -1);
		static void SetMaxSize(int width = -1, int height = -1);
		static void ForceAspectRatio(int width = -1, int height = -1);
		static void SetPosition(int x, int y);
		static void Centre();

		static bool ShouldClose();
		static bool GetMaximized();
		static float GetAspectRatio();		
		static const glm::ivec2& GetSize();
		static const glm::ivec2& GetPosition();
		static GLFWwindow* GetWindowHandle();
		static void GetNativeHandle(void* result);
		static glm::ivec2 GetNativeResolution(int monitor_index = 0);

	private:
		static void Init(bool transparent);
		static void Create();
		static void Cleanup();

		static void ErrorCallback(int error, const char* description);
		static void ResizeCallback(GLFWwindow* window_handle, int width, int height);
		static void MoveCallback(GLFWwindow* window_handle, int xpos, int ypos);
		static void FocusCallback(GLFWwindow* window_handle, int focused);
		static void PathDropCallback(GLFWwindow* window, int count, const char** paths);
		static void MaximizedCallback(GLFWwindow* window_handle, int maximized);
		static void MinimizedCallback(GLFWwindow* window_handle, int minimized);
	};
}