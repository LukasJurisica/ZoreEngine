#include "graphics/Texture.hpp"
#include "graphics/opengl/GLTexture.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	Texture2D* Texture2D::Create(const std::string& name) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLTexture2D(name);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	Texture2D* Texture2D::Create(uint width, uint height, TextureFormat format) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLTexture2D(width, height, format);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}
}