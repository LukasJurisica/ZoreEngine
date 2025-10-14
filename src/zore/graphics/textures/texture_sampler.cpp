#include "zore/graphics/textures/texture_sampler.hpp"
#include "zore/graphics/textures/texture_base.hpp"
#include <glad/glad.h>

namespace zore::Texture {

	//========================================================================
	//	Texture Sampler Class
	//========================================================================

	Sampler::Sampler() {
		glCreateSamplers(1, &m_id);
		SetWrapMode(Wrap::CLAMP);
		SetLODLevels(0, 0);
	}

	Sampler::~Sampler() {
		glDeleteSamplers(1, &m_id);
	}

	uint32_t Sampler::GetID() const {
		return m_id;
	}

	Sampler& Sampler::SetWrapMode(Wrap mode) {
		static const uint32_t s_wrap_mode_to_gl_wrap_mode[] = { GL_CLAMP_TO_EDGE, GL_REPEAT };

		uint32_t m = s_wrap_mode_to_gl_wrap_mode[static_cast<uint32_t>(mode)];
		glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R, m);
		glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, m);
		glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, m);
		return *this;
	}

	Sampler& Sampler::SetLODLevels(uint32_t min, uint32_t max) {
		glSamplerParameteri(m_id, GL_TEXTURE_MIN_LOD, min);
		glSamplerParameteri(m_id, GL_TEXTURE_MAX_LOD, max);
		return *this;
	}

	Sampler& Sampler::SetFilters(Filter min, Filter mag) {
		static const uint32_t s_sample_mode_to_gl_sample_mode[] = { GL_LINEAR, GL_NEAREST };

		glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, s_sample_mode_to_gl_sample_mode[static_cast<uint32_t>(mag)]);
		glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, s_sample_mode_to_gl_sample_mode[static_cast<uint32_t>(min)]);
		return *this;
	}

	void Sampler::Bind(uint32_t slot) const {
		glBindSampler(slot, m_id);
	}

	void Sampler::Bind(std::vector<uint32_t> slots) const {
		for (const uint32_t& slot : slots)
			glBindSampler(slot, m_id);
	}

	void Sampler::Bind(const std::string& slot) const {
		glBindSampler(Base::GetNamedTextureSlot(slot), m_id);
	}

	void Sampler::Bind(std::vector<std::string> slots) const {
		for (const std::string& slot : slots)
			glBindSampler(Base::GetNamedTextureSlot(slot), m_id);
	}
}