#pragma once

#include "zore/graphics/textures/texture_base.hpp"

namespace zore {

    //========================================================================
    //	Texture2D
    //========================================================================

    class Texture2D : public Texture::Base {
    public:
        Texture2D(Texture::Format format = Texture::Format::RGBA);
        Texture2D(void* data, uint32_t width, uint32_t height, Texture::Format format = Texture::Format::RGBA);
        Texture2D(const std::string& filename, Texture::Format format = Texture::Format::RGBA);
        Texture2D(Texture2D&&) = default;
        Texture2D& operator=(Texture2D&&) = default;
        ~Texture2D() = default;

        void Update(void* data);
        void Set(void* data, uint32_t width, uint32_t height);
        void Set(void* data, uint32_t width, uint32_t height, Texture::Format format);
        void Set(const std::string& filename, Texture::Format format = Texture::Format::RGBA);

        uint32_t GetWidth() { return m_width; }
        uint32_t GetHeight() { return m_height; }

    private:
        uint32_t m_width = 0;
        uint32_t m_height = 0;
    };
}