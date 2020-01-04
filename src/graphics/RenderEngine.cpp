#include "graphics/RenderEngine.hpp"
#include <glad/glad.h>
#include "graphics/opengl/OpenGLRenderAPI.hpp"

RenderEngine::RenderEngine(std::unique_ptr<Camera>& camera) : m_camera(camera) {
	switch (RenderAPI::getAPI()) {
	case RenderAPI::API::None:
		m_renderAPI = nullptr; break;
	case RenderAPI::API::OpenGL:
		m_renderAPI = new OpenGLRenderAPI(); break;
	default:
		Logger::error("Invalid RenderAPI"); m_renderAPI = nullptr;
	}
}

void RenderEngine::begin() {

}

void RenderEngine::submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader) {
	vertexArray->bind();
	shader->bind();
	shader->setUniformMat4("u_viewProjectionMatrix", m_camera->getViewProjectionMatrix());
	m_renderAPI->drawIndexed(vertexArray);
}

void RenderEngine::end() {

}

RenderEngine::~RenderEngine() {

}