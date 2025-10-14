#pragma once

#include "zore/graphics/textures/texture_base.hpp"
#include <vector>

namespace zore {

    class Texture2DArray : public Texture::Base {
    public:
        Texture2DArray(Texture::Format format = Texture::Format::RGBA);
        Texture2DArray(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format = Texture::Format::RGBA);
        Texture2DArray(const std::vector<std::string>& filenames, const std::string& root = "assets/", Texture::Format format = Texture::Format::RGBA);
        Texture2DArray(Texture2DArray&&) = default;
        Texture2DArray& operator=(Texture2DArray&&) = default;
        ~Texture2DArray() = default;

        void Update(void* data, uint32_t offset = 0, uint32_t count = ~0);
        void Set(void* data, uint32_t width, uint32_t height, uint32_t layers);
        void Set(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format);
        void Set(const std::vector<std::string>& filenames, const std::string& root = "assets/", Texture::Format format = Texture::Format::RGBA);

        uint32_t GetWidth() { return m_width; }
        uint32_t GetHeight() { return m_height; }
        uint32_t GetCount() { return m_layers; }

    private:
        uint32_t m_width = 0;
        uint32_t m_height = 0;
        uint32_t m_layers = 0;
    };
}