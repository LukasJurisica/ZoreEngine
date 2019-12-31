#include "graphics/opengl/OpenGLContext.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "utils/Logger.hpp"

OpenGLContext::OpenGLContext(void* window) : window(window) {}

void OpenGLContext::init() {
	glfwMakeContextCurrent((GLFWwindow*)window);
	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		Logger::error("Failed to initialize GLAD");

	Logger::info("Opengl Info:");
	Logger::info("  Vendor: " + std::string((char*)glGetString(GL_VENDOR)));
	Logger::info("  Renderer: " + std::string((char*)glGetString(GL_RENDERER)));
	Logger::info("  Version: " + std::string((char*)glGetString(GL_VERSION)));
}

void OpenGLContext::swapBuffers() {
	glfwSwapBuffers((GLFWwindow*)window);
}