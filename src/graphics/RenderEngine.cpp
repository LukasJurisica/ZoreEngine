#include "graphics/RenderEngine.hpp"
#include <glad/glad.h>

RenderAPI RenderEngine::renderAPI = RenderAPI::OpenGL;

RenderEngine::RenderEngine() {

}

void RenderEngine::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderEngine::setClearColour(glm::vec4 colour) {
	glClearColor(colour.r, colour.g, colour.b, colour.a);
}

RenderEngine::~RenderEngine() {

}