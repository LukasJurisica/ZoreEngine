#pragma once

#include "zore/utils/sized_integer.hpp"
#include <vector>
#include <string>

namespace zore::Texture {

	//========================================================================
	//	Texture Sampler
	//========================================================================

	class Sampler {
    public:
        enum class Wrap { CLAMP, REPEAT };
        enum class Filter { LINEAR, NEAREST };

	public:
        Sampler();
        Sampler(const Sampler&) = delete;
        Sampler(Sampler&&) = delete;
        Sampler& operator=(const Sampler&) = delete;
        Sampler& operator=(Sampler&&) = delete;
        ~Sampler();

        uint32_t GetID() const;
        Sampler& SetWrapMode(Wrap mode);
        Sampler& SetLODLevels(uint32_t min, uint32_t max);
        Sampler& SetFilters(Filter min, Filter mag);
		void Bind(uint32_t slot) const;
		void Bind(std::vector<uint32_t> slots) const;
		void Bind(const std::string& slot) const;
		void Bind(std::vector<std::string> slots) const;

	private:
		uint32_t m_id;
	};
}