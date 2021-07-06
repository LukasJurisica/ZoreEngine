#pragma once
#include "devices/Mouse.hpp"
#include "devices/Keyboard.hpp"
#include "glm/ext/vector_int2.hpp"

struct GLFWwindow;

namespace zore {

	//========================================================================
	//	Window Class
	//========================================================================

	class WindowListener;
	class Window {
	public:
		Window(const std::string& name, int width, int height);
		~Window();

		static void Init();
		static Window* Get(const std::string& name);
		static void Cleanup();

		void RegisterListener(WindowListener* listener);
		void UnregisterListener(WindowListener* listener);
		void Bind();
		void Update();
		bool ShouldClose();

		void SetTitle(const char* title);
		void SetFullscreen(bool value);
		void SetBorderless(bool value);
		void SetSize(int width, int height);
		void HideCursor(bool value);

		float GetAspectRatio();
		glm::ivec2& GetSize();
		Mouse* GetMouse();
		Keyboard* GetKeyboard();

	private:
		static void ErrorCallback(int error, const char* description);
		static void ResizeCallback(GLFWwindow* windowHandle, int width, int height);
		static void MoveCallback(GLFWwindow* windowHandle, int xpos, int ypos);
		static void FocusCallback(GLFWwindow* windowHandle, int focused);

		std::vector<WindowListener*> listeners;
		Mouse mouse;
		Keyboard keyboard;
		std::string name;
		glm::ivec2 size;
		glm::ivec2 position;
		GLFWwindow* windowHandle;
	};

	//========================================================================
	//	Window Listener Class
	//========================================================================

	class WindowListener {
		friend class Window;
	private:
		static void RemoveWaiter(WindowListener* listener);
		static void ClaimWaiters(Window* window, const std::string& name);
		static void ReleaseListeners(std::vector<WindowListener*>& listeners);

		virtual void OnWindowResize(int width, int height) {};
		virtual void OnWindowMove(int xpos, int ypos) {};
		virtual void OnWindowFocus(int focused) {};

	protected:
		WindowListener(const std::string& name = "");
		~WindowListener();

		std::string name;
		Window* window;
	};
}