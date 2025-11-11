#include "zore/graphics/textures/texture_base.hpp"
#include "zore/debug.hpp"
#include <unordered_map>
#include <glad/glad.h>

namespace zore::Texture {

    std::unordered_map<std::string, unsigned int> s_named_texture_slots;

	//========================================================================
	//	Texture Base
	//========================================================================

	Base::Base(Format format) : m_id(GL_INVALID_NAME), m_format(format), m_slot(0) {}

	Base::Base(Base&& other) noexcept {
		Move(other);
	}

	Base& Base::operator=(Base&& other) noexcept {
		if (this != &other)
			Move(other);
		return *this;
	}

	void Base::Move(Base& other) {
		m_id = other.m_id;
		m_format = other.m_format;
		m_slot = other.m_slot;
		other.m_id = GL_INVALID_NAME;
		other.m_format = Format::RGBA;
		other.m_slot = 0;
	}

	void Base::Init(uint32_t target) {
		if (m_id != GL_INVALID_NAME)
			glDeleteTextures(1, &m_id);
		glCreateTextures(target, 1, &m_id);
        m_bindless = false;
	}

	Base::~Base() {
		if (m_id != GL_INVALID_NAME)
			glDeleteTextures(1, &m_id);
	}

	uint32_t Base::GetID() const {
		return m_id;
	}

	void Base::Bind() const {
		glBindTextureUnit(m_slot, m_id);
	}

	void Base::Bind(uint32_t slot) {
		m_slot = slot;
		glBindTextureUnit(m_slot, m_id);
	}

	void Base::Bind(const std::string& slot) {
		m_slot = GetNamedTextureSlot(slot);
		glBindTextureUnit(m_slot, m_id);
	}

    //uint64_t Base::CreateHandle(const Sampler& sampler) const {
    //    m_bindless = true;
    //    return glGetTextureSamplerHandleARB(m_id, sampler.GetID());
    //}

	void Base::SetNamedTextureSlot(const std::string& name, uint32_t slot) {
		static constexpr uint32_t S_MAX_TEXTURE_SLOTS = 8;
		ENSURE(slot < S_MAX_TEXTURE_SLOTS, std::format("Attempted to create a named texture slot with invalid value (must be in the range 0 <= s < {}).", S_MAX_TEXTURE_SLOTS));
		s_named_texture_slots[name] = slot;
	}

	uint32_t Base::GetNamedTextureSlot(const std::string& name) {
		auto iter = s_named_texture_slots.find(name);
		if (iter != s_named_texture_slots.end())
			return iter->second;
		Logger::Error("Unknown texture slot name: " + name);
		return 0;
	}

	uint32_t Base::GetInternalFormat() {
        static constexpr uint32_t S_FORMAT_TO_INTERNAL_TYPE[] = {
            GL_R8  , GL_RG8  , GL_RGB8  , GL_RGBA8  , // R  , RG  , RGB  , RGBA
            GL_R8UI, GL_RG8UI, GL_RGB8UI, GL_RGBA8UI, // R8U, RG8U, RGB8U, RGBA8U
            GL_R32UI                                  // R32
        };
		return S_FORMAT_TO_INTERNAL_TYPE[static_cast<uint32_t>(m_format)];
	}

	uint32_t Base::GetBaseFormat() {
        static constexpr uint32_t S_FORMAT_TO_BASE_TYPE[] = {
            GL_RED        , GL_RG        , GL_RGB        , GL_RGBA        , // R  , RG  , RGB  , RGBA
            GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER, // R8U, RG8U, RGB8U, RGBA8U
            GL_RED_INTEGER                                                  // R32
        };
		return S_FORMAT_TO_BASE_TYPE[static_cast<uint32_t>(m_format)];
	}
}