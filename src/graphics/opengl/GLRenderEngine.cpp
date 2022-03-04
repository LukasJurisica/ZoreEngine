#include "graphics/opengl/GLRenderEngine.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	OpenGL Render Engine
	//========================================================================

	const unsigned int BufferTypeToGLBufferType[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT };
	const unsigned int MeshTopologyToGLMeshTopology[] = { GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };

	GLRenderEngine::GLRenderEngine() : clearMode(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT), topology(GL_TRIANGLES) {
		ENSURE(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Failed to initialize GLAD");

		glEnable(GL_BLEND); // TEMPORARY UNTIL I WRITE NEW BLENDING METHOD FOR FLUIDS
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	void GLRenderEngine::SetStencilTest(bool value) {
		value ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
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

	void GLRenderEngine::SetTopology(MeshTopology t) {
		topology = MeshTopologyToGLMeshTopology[static_cast<int>(t)];
	}

	void GLRenderEngine::Clear() {
		glClear(clearMode);
	}

	void GLRenderEngine::DrawLinear(unsigned int count, unsigned int offset) {
		if (count > 0)
			glDrawArrays(topology, offset, count);
	}

	void GLRenderEngine::DrawIndexed(unsigned int count, unsigned int offset) {
		if (count > 0)
			glDrawElements(topology, count, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(offset * sizeof(Index)));
	}

	void GLRenderEngine::DrawLinearInstanced(unsigned int vertexCount, unsigned int modelCount, unsigned int offset) {
		if (vertexCount > 0 && modelCount > 0)
			glDrawArraysInstanced(topology, offset, vertexCount, modelCount);
	}

	void GLRenderEngine::DrawIndexedInstanced(unsigned int indexCount, unsigned int modelCount, unsigned int offset) {
		if (indexCount > 0 && modelCount > 0)
			glDrawElementsInstanced(topology, indexCount, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(offset * sizeof(Index)), modelCount);
	}
}