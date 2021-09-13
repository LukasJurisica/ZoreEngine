#include "graphics/opengl/GLShader.hpp"
//#include "graphics/vulkan/VKShader.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Shader Interface
	//========================================================================

	Shader* Shader::Create(const std::string& name) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLShader(name);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}
}