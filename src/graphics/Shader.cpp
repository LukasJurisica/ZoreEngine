#include "graphics/Shader.hpp"
#include "graphics/opengl/OpenGLShader.hpp"
#include "graphics/RenderEngine.hpp"
#include "utils/Logger.hpp"


Shader* Shader::create(const char* vertpath, const char* fragpath) {
	switch (RenderEngine::getAPI()) {
	case RenderAPI::None:
		Logger::error("NULL RenderAPI is not currently supported"); return nullptr;
	case RenderAPI::OpenGL:
		return new OpenGLShader(vertpath, fragpath);
	default:
		Logger::error("Invalid RenderAPI"); return nullptr;
	}
}