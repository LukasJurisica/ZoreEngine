#include "graphics/opengl/OpenGLRenderAPI.hpp"

#include "glad/glad.h"

void OpenGLRenderAPI::init() {
	
}

void OpenGLRenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	glViewport(x, y, width, height);
}

void OpenGLRenderAPI::setClearColour(const glm::vec4& colour) {
	glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void OpenGLRenderAPI::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAPI::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const {
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}
