#include "graphics/Texture.hpp"
#include "graphics/opengl/GLTexture.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic 2D Texture Class
	//========================================================================

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

	void Texture2D::Init(uint width, uint height) {
		DEBUG_ENSURE(width * height, "Cannot create Texture with a dimension of 0.");
		this->width = width;
		this->height = height;
	}

	//========================================================================
	//	Platform Agnostic 2D Array Texture Class
	//========================================================================

	Texture2DArray* Texture2DArray::Create(const std::vector<std::string>& filenames, TextureFormat textureFormat) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLTexture2DArray(filenames, textureFormat);
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

	void Texture2DArray::Init(uint width, uint height, uint layers) {	
		DEBUG_ENSURE(layers * width * height, "Cannot create Texture Array with a dimension of 0.");
		this->width = width;
		this->height = height;
		this->layers = layers;
	}

	uint Texture2DArray::GetLayerCount() {
		return layers;
	}
}