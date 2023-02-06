#include "graphics/opengl/GLShader.hpp"
//#include "graphics/vulkan/VKShader.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Shader Interface
	//========================================================================

	Shader* Shader::Create(const std::string& name, const std::vector<ShaderDefineItem>& defines) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLShader(name, defines);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	ShaderStage Shader::GetStage(const std::string& stage) {
		if (stage == "vertex")
			return ShaderStage::VERTEX;
		else if (stage == "geometry")
			return ShaderStage::GEOMETRY;
		else if (stage == "fragment")
			return ShaderStage::FRAGMENT;

		Logger::Error("Invalid Shader Stage requested: " + stage);
		return ShaderStage::INVALID;
	}
}