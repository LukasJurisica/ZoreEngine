#pragma once

#include "zore/graphics/textures/texture_format.hpp"
#include "zore/graphics/textures/texture_sampler.hpp"
#include "zore/utils/sized_integer.hpp"
#include <string>

namespace zore::Texture {

    //========================================================================
    //	Texture Base
    //========================================================================

    class Base {
    protected:
        Base(Format format);
        Base(const Base&) = delete;
        Base(Base&&) noexcept;
        Base& operator=(const Base&) = delete;
        Base& operator=(Base&&) noexcept;
        void Move(Base& other);
        void Init(uint32_t target);
        ~Base();

    public:
        uint32_t GetID() const;
        void Bind() const;
        void Bind(uint32_t slot);
        void Bind(const std::string& slot);
        //uint64_t CreateHandle(const Sampler& sampler) const;

        static void SetNamedTextureSlot(const std::string& name, uint32_t slot);
        static uint32_t GetNamedTextureSlot(const std::string& name);

    protected:
        uint32_t GetInternalFormat();
        uint32_t GetBaseFormat();

    protected:
        uint32_t m_id;
        uint32_t m_slot;
        Format m_format;
        mutable bool m_bindless = false;
    };
}