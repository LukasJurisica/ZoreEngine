#pragma once

#include "zore/graphics/textures/texture_format.hpp"
#include <string>

namespace zore::Texture {

    //========================================================================
    //	Texture Data
    //========================================================================

    struct Data {
    public:
        Data() = default;
        Data(const std::string& path, Format requested_format = Format::RGBA);
        Data(const Data&) = delete;
		Data(Data&&) noexcept;
		Data& operator=(const Data&) = delete;
		Data& operator=(Data&&) noexcept;
        ~Data();

        void Move(Data& other);
        void Load(const std::string& path, Format requested_format = Format::RGBA);
        void Free();

    private:
        uint32_t GetChannelCount(Format format);

    public:
        uint8_t* data = nullptr;
        int width = 0;
        int height = 0;
        int channels = 0;
    };
}