#include "graphics/Texture.hpp"
#include "graphics/opengl/GLTexture.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"
#include <stb/stb_image.h>
#include <unordered_map>

namespace zore {

	//========================================================================
	//	Platform Agnostic Texture Utility
	//========================================================================

	std::unordered_map<std::string, unsigned int> namedTextureSlots;

	void Texture::SetTextureSlot(const std::string& name, unsigned int slot) {
		static constexpr unsigned int MAX_TEXTURE_SLOTS = 8;
		ENSURE(slot < MAX_TEXTURE_SLOTS, "Attempted to create a named texture slot with invalid value (must be in the range 0 <= s < 8).");
		namedTextureSlots[name] = slot;
	}

	unsigned int Texture::GetTextureSlot(const std::string& name) {
		auto iter = namedTextureSlots.find(name);
		ENSURE(iter != namedTextureSlots.end(), "Invalid texture slot name: " + name);
		return iter->second;
	}

	//========================================================================
	//	Platform Agnostic Texture Sampler Class
	//========================================================================

	Texture::Sampler* Texture::Sampler::Create(SampleMode mode) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLSampler(mode);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	void Texture::Sampler::Bind(const std::string& slot) {
		Bind(GetTextureSlot(slot));
	}

	//========================================================================
	//	Platform Agnostic 2D Texture Class
	//========================================================================

	Texture2D* Texture2D::Create(const std::string& filename, Texture::Format format) {
		int w, h, c;
		Texture2D* texture = nullptr;
		ubyte* data = stbi_load(filename.c_str(), &w, &h, &c, static_cast<uint>(format) + 1);
		ENSURE(data, "Could not load texture: " + filename);

		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			texture = new GLTexture2D(w, h, format, data);
			break;
		default:
			throw ZORE_EXCEPTION("Invalid RenderAPI");
		}
		
		stbi_image_free(data);
		return texture;
	}

	Texture2D* Texture2D::Create(uint width, uint height, Texture::Format format, void* data) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLTexture2D(width, height, format, data);
		default:
			throw ZORE_EXCEPTION("Invalid RenderAPI");
		}
	}

	void Texture2D::Bind(const std::string& slot) {
		Bind(Texture::GetTextureSlot(slot));
	}

	void Texture2D::Init(uint w, uint h) {
		DEBUG_ENSURE(w * h, "Cannot create Texture2D with a dimension of 0.");
		width = w;
		height = h;
	}

	//========================================================================
	//	Platform Agnostic 2D Array Texture Class
	//========================================================================

	Texture2DArray* Texture2DArray::Create(const std::vector<std::string>& filenames, const std::string& root, Texture::Format format) {
		// Ensure positive number of filenames are requested
		uint layers = static_cast<uint>(filenames.size());
		DEBUG_ENSURE(layers > 0, "No filenames passed to Texture2DArray::Create.");

		// Load the first image, and initialize the Texture2D Array
		int w, h, c;
		GLTexture2DArray* texture = nullptr;
		std::string path = root + filenames[0];
		ubyte* data = stbi_load(path.c_str(), &w, &h, &c, static_cast<uint>(format) + 1);
		ENSURE(data, "Could not load texture: " + path);
		
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			texture = new GLTexture2DArray(w, h, layers, format, data, 1);
			break;
		default:
			throw ZORE_EXCEPTION("Invalid RenderAPI");
		}

		// Load the remaining images
		stbi_image_free(data);
		for (int i = 1; i < filenames.size(); i++) {
			path = root + filenames[i];
			ubyte* data = stbi_load(path.c_str(), &w, &h, &c, static_cast<int>(format) + 1);
			ENSURE(data, "Could not load texture: " + path);
			ENSURE(w == texture->width && h == texture->height, "The following texture cannot be added to the texture array as it has different dimensions: " + path);
			texture->SetTextureData(i, 1, data);
			stbi_image_free(data);
		}
		return texture;
	}

	Texture2DArray* Texture2DArray::Create(uint width, uint height, uint layers, Texture::Format format, void* data) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLTexture2DArray(width, height, layers, format, data, layers);
		default:
			throw ZORE_EXCEPTION("Invalid RenderAPI");
		}
	}

	void Texture2DArray::Bind(const std::string& slot) {
		Bind(Texture::GetTextureSlot(slot));
	}

	void Texture2DArray::Init(uint w, uint h, uint l) {
		DEBUG_ENSURE(w * h * l, "Cannot create Texture Array with a dimension of 0.");
		width = w;
		height = h;
		layers = l;
	}
}