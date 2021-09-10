#include "graphics/Texture.hpp"
#include "graphics/opengl/GLTexture.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	Texture2D* Texture2D::Create(const std::string& name) {
		switch (RenderEngine::GetAPI()) {
		case API::OpenGL:
			return new GLTexture2D(name);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	Texture2D* Texture2D::Create(uint width, uint height, uint channels) {
		switch (RenderEngine::GetAPI()) {
		case API::OpenGL:
			return new GLTexture2D(width, height, CCtoGLTT(channels));
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}
}