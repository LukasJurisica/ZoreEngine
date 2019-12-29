#include "graphics/opengl/OpenGLContext.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "utils/Logger.hpp"

OpenGLContext::OpenGLContext(GLFWwindow* window) : window(window) {}

void OpenGLContext::init() {
	glfwMakeContextCurrent(window);
	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		Logger::error("Failed to initialize GLAD");
}

void OpenGLContext::swapBuffers() {
	glfwSwapBuffers(window);
}