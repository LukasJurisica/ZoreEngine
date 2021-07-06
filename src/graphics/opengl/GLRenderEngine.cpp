#include "graphics/opengl/GLRenderEngine.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	OpenGL Render Engine
	//========================================================================

	void GLRenderEngine::Init() {
		ENSURE(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Failed to initialize GLAD");
	}

	GLRenderEngine::GLRenderEngine() {

	}

	void GLRenderEngine::SetClearColour(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void GLRenderEngine::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
		glViewport(x, y, width, height);
	}

	void GLRenderEngine::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderEngine::DrawLinear(unsigned int count, unsigned int offset) {
		glDrawArrays(GL_TRIANGLES, offset, count);
	}

	void GLRenderEngine::DrawIndexed(unsigned int count, unsigned int offset) {
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(offset));
	}
}