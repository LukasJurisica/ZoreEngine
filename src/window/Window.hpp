#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "graphics/Context.hpp"
#include "window/InputCodes.hpp"

struct GLFWwindow;

class Window {
public:
	enum class ScreenMode {
		Fullscreen,
		Windowed,
		Borderless
	};
	struct WindowData {
		int width;
		int height;
		ScreenMode screenMode;
		const char* title;
		bool vSync;
		void* application;
	};

	Window(std::map<std::string, int> options, void* application);
	~Window();

	void init();

	inline int getWidth() const { return m_windowData.width; }
	inline int getHeight() const { return m_windowData.height; }
	inline float getAspectRatio() const { return (float)m_windowData.width / (float)m_windowData.height; }
	bool shouldClose();
	bool keyPressed(int key);
	bool mousePressed(int button);

	void update();
	void setResolution(int width, int height);
	void updateResolution(int width, int height);
	void setVSync(bool enabled);

private:
	WindowData m_windowData;
	GLFWwindow* m_window;
	Context* m_context;
};

namespace WindowCallback {
	void errorCallback(int error, const char* description);
	void windowSizeCallback(GLFWwindow* window, int width, int height);
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}