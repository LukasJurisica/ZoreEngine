#pragma once

#include "zore/graphics/Texture.hpp"

#include <string>

namespace zore::UI {

    //========================================================================
    //	Font Class
    //========================================================================

    class Font {
    public:
        Font(const std::string& path, Texture::Format format = Texture::Format::RGBA);
        Font(Font&&) noexcept = default;
        Font& operator=(Font&&) noexcept = default;
		~Font() = default;
        static Font& Create(const std::string& name, const std::string& path, Texture::Format format = Texture::Format::RGBA);
        static Font& Create(const std::string& path, Texture::Format format = Texture::Format::RGBA);
        static Font* Get(const std::string& name);
        Texture2DArray& GetTextureArray() { return m_texture_array; }

    private:
		Texture2DArray m_texture_array;
    };
}