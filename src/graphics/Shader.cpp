#include "graphics/opengl/OpenGLShader.hpp"
#include "graphics/RenderAPI.hpp"
#include "utils/Logger.hpp"

std::shared_ptr<Shader> Shader::create(const std::string& filename) {
	switch (RenderAPI::getAPI()) {
	case RenderAPI::API::None:
		Logger::error("NULL RenderAPI is not currently supported"); return nullptr;
	case RenderAPI::API::OpenGL:
		return std::make_shared<OpenGLShaderProgram>(filename);
	default:
		Logger::error("Invalid RenderAPI"); return nullptr;
	}
}