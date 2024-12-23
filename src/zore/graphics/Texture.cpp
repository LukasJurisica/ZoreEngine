#include "zore/graphics/Texture.hpp"
#include "zore/Debug.hpp"

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
		static const uint32_t s_wrap_mode_to_gl_wrap_mode[] = { GL_CLAMP_TO_EDGE, GL_REPEAT };

		uint32_t m = s_wrap_mode_to_gl_wrap_mode[static_cast<uint32_t>(mode)];
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
		static const uint32_t s_sample_mode_to_gl_sample_mode[] = { GL_LINEAR, GL_NEAREST };

		glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, s_sample_mode_to_gl_sample_mode[static_cast<uint32_t>(mag)]);
		glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, s_sample_mode_to_gl_sample_mode[static_cast<uint32_t>(min)]);
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

	static constexpr uint32_t S_INVALID_TEXTURE_ID = ~0;
	static constexpr uint32_t S_FORMAT_TO_COUNT[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA, GL_RED, GL_RG, GL_RGB, GL_RGBA };
	static constexpr uint32_t S_FORMAT_TO_TYPE[] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8, GL_R8UI, GL_RG8UI, GL_RGB8UI, GL_RGBA8UI };
	std::unordered_map<std::string, unsigned int> s_named_texture_slots;

	Texture::Texture(uint32_t target, Format format) :
		m_id(S_INVALID_TEXTURE_ID), m_target(target), m_format(static_cast<uint32_t>(format)), m_slot(0) {}

	Texture::Texture(Texture&& other) noexcept {
		Swap(other);
	}

	Texture& Texture::operator=(Texture&& other) noexcept {
		if (this != &other)
			Swap(other);
		return *this;
	}

	void Texture::Swap(Texture& other) {
		m_id = other.m_id;
		m_format = other.m_format;
		m_slot = other.m_slot;
		other.m_id = S_INVALID_TEXTURE_ID;
		other.m_format = static_cast<uint32_t>(Format::RGBA);
		other.m_slot = 0;glDeleteTextures(1, &m_id);
	}

	Texture::~Texture() {
		if (m_id != S_INVALID_TEXTURE_ID)
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
		static constexpr uint32_t S_MAX_TEXTURE_SLOTS = 8;
		ENSURE(slot < S_MAX_TEXTURE_SLOTS, "Attempted to create a named texture slot with invalid value (must be in the range 0 <= s < 8).");
		s_named_texture_slots[name] = slot;
	}

	uint32_t Texture::GetNamedTextureSlot(const std::string& name) {
		auto iter = s_named_texture_slots.find(name);
		if (iter != s_named_texture_slots.end())
			return iter->second;
		Logger::Error("Invalid texture slot name: " + name);
		return 0;
	}

	//========================================================================
	//	2D Texture Class
	//========================================================================

	Texture2D::Texture2D(Texture::Format format) : Texture(GL_TEXTURE_2D, format) {}

	Texture2D::Texture2D(void* data, uint32_t width, uint32_t height, Texture::Format format) : Texture(GL_TEXTURE_2D, format), m_width(width), m_height(height) {
		Set(data, width, height, format);
	}

	Texture2D::Texture2D(const std::string& filename, Texture::Format format) : Texture(GL_TEXTURE_2D, format) {
		Set(filename, format);
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept : Texture(std::move(other)) {
		Swap(other);
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
		if (this != &other) {
			Texture::operator=(std::move(other));
			Swap(other);
		}
		return *this;
	}

	void Texture2D::Swap(Texture2D& other) {
		m_width = other.m_width;
		m_height = other.m_height;
		other.m_width = 0;
		other.m_height = 0;
	}

	void Texture2D::Update(void* data) {
		if (data)
			glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, S_FORMAT_TO_COUNT[m_format], GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Set(void* data, uint32_t width, uint32_t height) {
		ENSURE(width * height != 0, "Cannot create Texture2D with 0 area (requested Width or Height are 0)");
		if (m_id != S_INVALID_TEXTURE_ID)
			glDeleteTextures(1, &m_id);
		m_width = width;
		m_height = height;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		glTextureStorage2D(m_id, 1, S_FORMAT_TO_TYPE[m_format], m_width, m_height);
		Update(data);
	}

	void Texture2D::Set(void* data, uint32_t width, uint32_t height, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		Set(data, width, height);
	}

	void Texture2D::Set(const std::string& filename, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		int width, height, channels;
		uint8_t* data = stbi_load(filename.c_str(), &width, &height, &channels, (m_format % 4) + 1);
		ENSURE(data, "Could not load texture: " + filename);

		Set(data, width, height);
		stbi_image_free(data);
	}

	//========================================================================
	//  2D Array Texture Class
	//========================================================================

	Texture2DArray::Texture2DArray(Texture::Format format) : Texture(GL_TEXTURE_2D_ARRAY, format) {}

	Texture2DArray::Texture2DArray(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format)
		: Texture(GL_TEXTURE_2D_ARRAY, format), m_width(width), m_height(height), m_layers(layers) {
		Set(data, width, height, layers, format);
	}

	Texture2DArray::Texture2DArray(const std::vector<std::string>& filenames, const std::string& root, Texture::Format format)
		: Texture(GL_TEXTURE_2D_ARRAY, format) {
		Set(filenames, root, format);
	}

	Texture2DArray::Texture2DArray(Texture2DArray&& other) noexcept : Texture(std::move(other)) {
		Swap(other);
	}

	Texture2DArray& Texture2DArray::operator=(Texture2DArray&& other) noexcept {
		if (this != &other) {
			Texture::operator=(std::move(other));
			Swap(other);
		}
		return *this;
	}

	void Texture2DArray::Swap(Texture2DArray& other) {
		m_width = other.m_width;
		m_height = other.m_height;
		m_layers = other.m_layers;
		other.m_width = 0;
		other.m_height = 0;
		other.m_layers = 0;
	}

	void Texture2DArray::Update(void* data, uint32_t offset, uint32_t count) {
		if (data && count > 0) {
			count = ((offset + count) <= m_layers) ? count : m_layers - offset;
			glTextureSubImage3D(m_id, 0, 0, 0, offset, m_width, m_height, count, S_FORMAT_TO_COUNT[m_format], GL_UNSIGNED_BYTE, data);
		}
	}

	void Texture2DArray::Set(void* data, uint32_t width, uint32_t height, uint32_t layers) {
		ENSURE(width * height * layers != 0, "Cannot create Texture2DArray with 0 area (requested Width, Height or Layers are 0)");
		if (m_id != S_INVALID_TEXTURE_ID)
			glDeleteTextures(1, &m_id);
		m_width = width;
		m_height = height;
		m_layers = layers;
		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_id);
		glTextureStorage3D(m_id, 1, S_FORMAT_TO_TYPE[m_format], m_width, m_height, m_layers);
		Update(data, 0, m_layers);
	}

	void Texture2DArray::Set(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		Set(data, width, height, layers);
	}

	void Texture2DArray::Set(const std::vector<std::string>& filenames, const std::string& root, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		// Load the first image, and initialize the Texture2D Array
		int width, height, channels;
		std::string path = root + filenames[0];
		uint8_t* data = stbi_load(path.c_str(), &width, &height, &channels, m_format + 1);
		ENSURE(data, "Could not load texture: " + path);
		m_width = width;
		m_height = height;

		Update(data, 0, 1);
		stbi_image_free(data);

		// Load the remaining images
		for (int i = 1; i < filenames.size(); i++) {
			path = root + filenames[i];
			uint8_t* data = stbi_load(path.c_str(), &width, &height, &channels, m_format + 1);
			ENSURE(data, "Could not load texture: " + path);
			ENSURE(width == m_width && height == m_height, "The following texture cannot be added to the texture array as it has different dimensions: " + path);
			Update(data, i, 1);
			stbi_image_free(data);
		}
	}
}