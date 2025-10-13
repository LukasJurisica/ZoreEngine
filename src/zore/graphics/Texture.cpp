#include "zore/graphics/texture.hpp"
#include "zore/debug.hpp"
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

	static constexpr uint32_t S_FORMAT_TO_BASE_TYPE[] = {
		GL_RED        , GL_RG        , GL_RGB        , GL_RGBA        , // R  , RG  , RGB  , RGBA
		GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER, // R8U, RG8U, RGB8U, RGBA8U
		GL_RED_INTEGER                                                  // R32
	};
	static constexpr uint32_t S_FORMAT_TO_INTERNAL_TYPE[] = {
		GL_R8  , GL_RG8  , GL_RGB8  , GL_RGBA8  , // R  , RG  , RGB  , RGBA
		GL_R8UI, GL_RG8UI, GL_RGB8UI, GL_RGBA8UI, // R8U, RG8U, RGB8U, RGBA8U
		GL_R32UI                                  // R32
	};
	static constexpr uint32_t S_FORMAT_TO_COUNT[] = {
		1, 2, 3, 4, // R  , RG  , RGB  , RGBA
		1, 2, 3, 4, // R8U, RG8U, RGB8U, RGBA8U
		1		    // R32
	};
	std::unordered_map<std::string, unsigned int> s_named_texture_slots;

	Texture::Texture(Format format) : m_id(GL_INVALID_NAME), m_format(static_cast<uint32_t>(format)), m_slot(0) {}

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
		other.m_id = GL_INVALID_NAME;
		other.m_format = static_cast<uint32_t>(Format::RGBA);
		other.m_slot = 0;
	}

	void Texture::Init(uint32_t target) {
		if (m_id != GL_INVALID_NAME)
			glDeleteTextures(1, &m_id);
		glCreateTextures(target, 1, &m_id);
	}

	Texture::~Texture() {
		if (m_id != GL_INVALID_NAME)
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

	Texture::Image Texture::LoadFromFile(const std::string& path, Format format) {
		Image result;
		result.data = stbi_load(path.c_str(), &result.width, &result.height, &result.channels, S_FORMAT_TO_COUNT[static_cast<uint32_t>(format)]);
		ENSURE(result.data, "Could not load texture: " + path);
		return result;
	}

	void Texture::FreeImage(Texture::Image image) {
		stbi_image_free(image.data);
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

	Texture2D::Texture2D(Texture::Format format) : Texture(format) {}

	Texture2D::Texture2D(void* data, uint32_t width, uint32_t height, Texture::Format format) : Texture(format), m_width(width), m_height(height) {
		Set(data, width, height, format);
	}

	Texture2D::Texture2D(const std::string& filename, Texture::Format format) : Texture(format) {
		Set(filename, format);
	}

	void Texture2D::Update(void* data) {
		if (data)
			glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, S_FORMAT_TO_BASE_TYPE[m_format], GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Set(void* data, uint32_t width, uint32_t height) {
		ENSURE(width * height != 0, "Cannot create Texture2D with 0 area (requested Width or Height are 0)");
		m_width = width;
		m_height = height;
		Init(GL_TEXTURE_2D);
		glTextureStorage2D(m_id, 1, S_FORMAT_TO_INTERNAL_TYPE[m_format], m_width, m_height);
		Update(data);
	}

	void Texture2D::Set(void* data, uint32_t width, uint32_t height, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		Set(data, width, height);
	}

	void Texture2D::Set(const std::string& filename, Texture::Format format) {
		Image image = LoadFromFile(filename, format);
		Set(image.data, image.width, image.height, format);
		FreeImage(image);
	}

	//========================================================================
	//  2D Array Texture Class
	//========================================================================

	Texture2DArray::Texture2DArray(Texture::Format format) : Texture(format) {}

	Texture2DArray::Texture2DArray(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format) : Texture(format) {
		Set(data, width, height, layers, format);
	}

	Texture2DArray::Texture2DArray(const std::vector<std::string>& filenames, const std::string& root, Texture::Format format) : Texture(format) {
		Set(filenames, root, format);
	}

	void Texture2DArray::Update(void* data, uint32_t offset, uint32_t count) {
		if (data && count > 0) {
			count = ((offset + count) <= m_layers) ? count : m_layers - offset;
			glTextureSubImage3D(m_id, 0, 0, 0, offset, m_width, m_height, count, S_FORMAT_TO_BASE_TYPE[m_format], GL_UNSIGNED_BYTE, data);
		}
	}

	void Texture2DArray::Set(void* data, uint32_t width, uint32_t height, uint32_t layers) {
		ENSURE(width * height * layers != 0, "Cannot create Texture2DArray with 0 area (requested Width, Height or Layers are 0)");
		m_width = width;
		m_height = height;
		m_layers = layers;
		Init(GL_TEXTURE_2D_ARRAY);
		glTextureStorage3D(m_id, 1, S_FORMAT_TO_INTERNAL_TYPE[m_format], m_width, m_height, m_layers);
		Update(data, 0, m_layers);
	}

	void Texture2DArray::Set(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format) {
		m_format = static_cast<uint32_t>(format);
		Set(data, width, height, layers);
	}

	void Texture2DArray::Set(const std::vector<std::string>& filenames, const std::string& root, Texture::Format format) {
		// Load the first image, and initialize the Texture2D Array
		Image image = LoadFromFile(root + filenames[0], format);
		Set(nullptr, image.width, image.height, static_cast<uint32_t>(filenames.size()), format);
		Update(image.data, 0, 1);
		FreeImage(image);

		// Load the remaining images
		for (int i = 1; i < filenames.size(); i++) {
			image = LoadFromFile(root + filenames[i], format);
			ENSURE(image.width == m_width && image.height == m_height, "The following texture cannot be added to the texture array as it has different dimensions: " + root + filenames[i]);
			Update(image.data, i, 1);
			FreeImage(image);
		}
	}
}