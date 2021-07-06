#include "graphics/opengl/GLShader.hpp"
//#include "graphics/vulkan/VKShader.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Shader Interface
	//========================================================================

	unsigned int ShaderDataTypeSize(ShaderDataType type, unsigned int count) {
		switch (type) {
		case ShaderDataType::Int:
			return (4 * count);
		case ShaderDataType::Float:
			return (4 * count);
		case ShaderDataType::Mat:
			return (4 * count * count);
		case ShaderDataType::Bool:
			return (1 * count);
		case ShaderDataType::Byte:
		case ShaderDataType::UByte:
			return (1 * count);
		default:
			throw ZORE_EXCEPTION("Invalid ShaderDataType");
			return (0);
		}
	}

	Shader* Shader::Create(const std::string& name) {
		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			return new GLShader(name);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	void Shader::UnBind() {
		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			GLShader::UnBind();
			break;
		case API::Vulkan:
			//VKShader::UnBind();
			break;
		}
	}
}