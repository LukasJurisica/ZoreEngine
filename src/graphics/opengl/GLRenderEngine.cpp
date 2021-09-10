#include "graphics/opengl/GLRenderEngine.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	OpenGL Render Engine
	//========================================================================

	const unsigned int GLRenderEngine::BufferTypeToGLBufferType[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT };

	GLRenderEngine::GLRenderEngine() : clearMode(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) {
		ENSURE(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Failed to initialize GLAD");
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
	}

	void GLRenderEngine::SetViewport(unsigned int width, unsigned int height, unsigned int x, unsigned int y) {
		glViewport(x, y, width, height);
	}

	void GLRenderEngine::SetBackFaceCulling(bool value) {
		value ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	}

	void GLRenderEngine::SetDepthTest(bool value) {
		value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void GLRenderEngine::SetWireframe(bool value) {
		glPolygonMode(GL_FRONT_AND_BACK, value ? GL_LINE : GL_FILL);
	}

	void GLRenderEngine::SetVSync(bool value) {
		glfwSwapInterval(value);
	}

	void GLRenderEngine::SetClearColour(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void GLRenderEngine::SetClearMode(const std::vector<BufferType>& buffers) {
		clearMode = 0u;
		for (const BufferType& b : buffers)
			clearMode |= BufferTypeToGLBufferType[static_cast<int>(b)];
	}

	void GLRenderEngine::Clear() {
		glClear(clearMode);
	}

	void GLRenderEngine::DrawLinear(unsigned int count, unsigned int offset) {
		glDrawArrays(GL_TRIANGLES, offset, count);
	}

	void GLRenderEngine::DrawIndexed(unsigned int count, unsigned int offset) {
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(offset * sizeof(Index)));
	}
}