#include "graphics/Texture.hpp"
#include "graphics/opengl/GLTexture.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic 2D Texture Class
	//========================================================================

	Texture2D::Texture2D(uint width, uint height) : width(width), height(height) {
		DEBUG_ENSURE(width * height, "Cannot create Texture with a dimension of 0.");
	}

	Texture2D* Texture2D::Create(const std::string& name, TextureFormat textureFormat) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLTexture2D(name, textureFormat);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	Texture2D* Texture2D::Create(uint width, uint height, TextureFormat textureFormat, void* data) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLTexture2D(width, height, textureFormat, data);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	//========================================================================
	//	Platform Agnostic 2D Array Texture Class
	//========================================================================

	Texture2DArray::Texture2DArray(uint width, uint height, uint layers) : width(width), height(height), layers(layers) {
		DEBUG_ENSURE(layers * width * height, "Cannot create Texture Array with a dimension of 0.");
	}

	Texture2DArray* Texture2DArray::Create(uint width, uint height, const std::vector<std::string>& filenames, TextureFormat textureFormat) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLTexture2DArray(width, height, filenames, textureFormat);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	Texture2DArray* Texture2DArray::Create(uint width, uint height, uint layers, void* data, TextureFormat textureFormat) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLTexture2DArray(width, height, layers, data, textureFormat);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}
}