#include "zore/graphics/Texture.hpp"
#include "zore/debug/Debug.hpp"
#include <stb/stb_image.h>
#include <unordered_map>
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	Texture Sampler Class
	//========================================================================

	TextureSampler::TextureSampler() {
		glCreateSamplers(1, &m_id);
		SetWrapMode(Wrap::CLAMP);
		SetLODLevels(0, 0);
	}

	TextureSampler::~TextureSampler() {
		glDeleteSamplers(1, &m_id);
	}

	uint32_t TextureSampler::GetID() const {
		return m_id;
	}

	TextureSampler& TextureSampler::SetWrapMode(Wrap mode) {
		static const uint32_t WrapModeToGLWrapMode[] = { GL_CLAMP_TO_EDGE, GL_REPEAT };

		uint32_t m = WrapModeToGLWrapMode[static_cast<uint32_t>(mode)];
		glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R, m);
		glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, m);
		glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, m);
		return *this;
	}

	TextureSampler& TextureSampler::SetLODLevels(uint32_t min, uint32_t max) {
		glSamplerParameteri(m_id, GL_TEXTURE_MIN_LOD, min);
		glSamplerParameteri(m_id, GL_TEXTURE_MAX_LOD, max);
		return *this;
	}

	TextureSampler& TextureSampler::SetFilters(Filter min, Filter mag) {
		static const uint32_t SampleModeToGLSampleMode[] = { GL_LINEAR, GL_NEAREST };

		glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, SampleModeToGLSampleMode[static_cast<uint32_t>(mag)]);
		glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, SampleModeToGLSampleMode[static_cast<uint32_t>(min)]);
		return *this;
	}

	void TextureSampler::Bind(uint32_t slot) const {
		glBindSampler(slot, m_id);
	}

	void TextureSampler::Bind(std::vector<uint32_t> slots) const {
		for (const uint32_t& slot : slots)
			glBindSampler(slot, m_id);
	}

	void TextureSampler::Bind(const std::string& slot) const {
		glBindSampler(Texture::GetNamedTextureSlot(slot), m_id);
	}

	void TextureSampler::Bind(std::vector<std::string> slots) const {
		for (const std::string& slot : slots)
			glBindSampler(Texture::GetNamedTextureSlot(slot), m_id);
	}

	//========================================================================
	//	Texture Base Class
	//========================================================================

	static const uint32_t TextureFormatToGLBaseFormat[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
	static const uint32_t TextureFormatToGLSizedFormat[] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };
	std::unordered_map<std::string, unsigned int> namedTextureSlots;

	Texture::Texture(uint32_t target, Format format) : m_format(static_cast<uint32_t>(format)), m_slot(0) {
		glCreateTextures(target, 1, &m_id);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_id);
	}

	uint32_t Texture::GetID() const {
		return m_id;
	}

	void Texture::Bind() const {
		glBindTextureUnit(m_slot, m_id);
	}

	void Texture::Bind(uint32_t slot) {
		m_slot = slot;
		glBindTextureUnit(m_slot, m_id);
	}

	void Texture::Bind(const std::string& slot) {
		m_slot = GetNamedTextureSlot(slot);
		glBindTextureUnit(m_slot, m_id);
	}

	void Texture::SetNamedTextureSlot(const std::string& name, uint32_t slot) {
		static constexpr uint32_t MAX_TEXTURE_SLOTS = 8;
		ENSURE(slot < MAX_TEXTURE_SLOTS, "Attempted to create a named texture slot with invalid value (must be in the range 0 <= s < 8).");
		namedTextureSlots[name] = slot;
	}

	uint32_t Texture::GetNamedTextureSlot(const std::string& name) {
		auto iter = namedTextureSlots.find(name);
		if (iter != namedTextureSlots.end())
			return iter->second;
		Logger::Error("Invalid texture slot name: " + name);
		return 0;
	}

	//========================================================================
	//	2D Texture Class
	//========================================================================

	Texture2D::Texture2D() : Texture(GL_TEXTURE_2D, Texture::Format::RGBA), m_width(0), m_height(0) {}

	Texture2D::Texture2D(uint32_t width, uint32_t height, Texture::Format format) : Texture(GL_TEXTURE_2D, format) {
		Init(width, height);
	}

	Texture2D::Texture2D(const std::string& filename, Texture::Format format) : Texture(GL_TEXTURE_2D, format) {
		Set(filename, format);
	}

	void Texture2D::SetData(void* data) {
		if (data)
			glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, TextureFormatToGLBaseFormat[m_format], GL_UNSIGNED_BYTE, data);
	}

	Texture2D& Texture2D::SetSize(uint32_t width, uint32_t height) {
		Init(width, height);
		return *this;
	}

	Texture2D& Texture2D::SetSize(uint32_t width, uint32_t height, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		return SetSize(width, height);
	}

	void Texture2D::Set(const std::string& filename, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		int width, height, channels;
		uint8_t* data = stbi_load(filename.c_str(), &width, &height, &channels, m_format + 1);
		ENSURE(data, "Could not load texture: " + filename);

		Init(width, height);
		SetData(data);
		stbi_image_free(data);
	}

	void Texture2D::Init(uint32_t width, uint32_t height) {
		ENSURE(width * height != 0, "Cannot create Texture2D with 0 area (requested Width or Height are 0)");
		
		if (m_width * m_height) {
			glDeleteTextures(1, &m_id);
			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		}

		m_width = width;
		m_height = height;
		glTextureStorage2D(m_id, 1, TextureFormatToGLSizedFormat[m_format], m_width, m_height);
	}

	//========================================================================
	//  2D Array Texture Class
	//========================================================================

	Texture2DArray::Texture2DArray() : Texture(GL_TEXTURE_2D_ARRAY, Texture::Format::RGBA) {}

	Texture2DArray::Texture2DArray(uint32_t width, uint32_t height, uint32_t layers, Texture::Format format) : Texture(GL_TEXTURE_2D_ARRAY, format) {
		Init(width, height, layers);
	}

	Texture2DArray::Texture2DArray(const std::vector<std::string>& filenames, const std::string& root, Texture::Format format) : Texture(GL_TEXTURE_2D_ARRAY, format) {
		Set(filenames, root, format);
	}

	void Texture2DArray::SetData(uint32_t first, uint32_t count, void* data) {
		if (data && count > 0) {
			count = ((first + count) <= m_layers) ? count : m_layers - first;
			glTextureSubImage3D(m_id, 0, 0, 0, first, m_width, m_height, count, TextureFormatToGLBaseFormat[m_format], GL_UNSIGNED_BYTE, data);
		}
	}

	Texture2DArray& Texture2DArray::SetSize(uint32_t width, uint32_t height, uint32_t layers) {
		Init(width, height, layers);
		return *this;
	}

	Texture2DArray& Texture2DArray::SetSize(uint32_t width, uint32_t height, uint32_t layers, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		return SetSize(width, height, layers);
	}

	void Texture2DArray::Set(const std::vector<std::string>& filenames, const std::string& root, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		// Load the first image, and initialize the Texture2D Array
		int width, height, channels;
		std::string path = root + filenames[0];
		uint8_t* data = stbi_load(path.c_str(), &width, &height, &channels, m_format + 1);
		ENSURE(data, "Could not load texture: " + path);

		Init(width, height, static_cast<uint32_t>(filenames.size()));
		SetData(0, 1, data);
		stbi_image_free(data);

		// Load the remaining images
		for (int i = 1; i < filenames.size(); i++) {
			path = root + filenames[i];
			uint8_t* data = stbi_load(path.c_str(), &width, &height, &channels, m_format + 1);
			ENSURE(data, "Could not load texture: " + path);
			ENSURE(width == m_width && height == m_height, "The following texture cannot be added to the texture array as it has different dimensions: " + path);
			SetData(i, 1, data);
			stbi_image_free(data);
		}
	}

	void Texture2DArray::Init(uint32_t width, uint32_t height, uint32_t layers) {
		ENSURE(width * height * layers != 0, "Cannot create Texture2DArray with 0 area (requested Width, Height or Layers are 0)");

		if (m_width * m_height * m_layers) {
			glDeleteTextures(1, &m_id);
			glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_id);
		}
		
		m_width = width;
		m_height = height;
		m_layers = layers;
		glTextureStorage3D(m_id, 1, TextureFormatToGLSizedFormat[m_format], m_width, m_height, m_layers);
	}
}