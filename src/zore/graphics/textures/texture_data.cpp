#include "zore/graphics/textures/texture_data.hpp"
#include "zore/graphics/textures/texture_base.hpp"
#include <stb/stb_image.h>

namespace zore::Texture {

    Data::Data(const std::string& path, Format requested_format) {
        Load(path, requested_format);
    }

    Data::Data(Data&& other) noexcept {
        Move(other);
    }

    Data& Data::operator=(Data&& other) noexcept {
        if (this != &other)
            Move(other);
        return *this;
    }

    Data::~Data() {
        Free();
    }

    void Data::Move(Data& other) {
        data = other.data;
        width = other.width;
        height = other.height;
        channels = other.channels;
        other.data = nullptr;
        other.width = 0;
        other.height = 0;
        other.channels = 0;
    }

    void Data::Load(const std::string& path, Format requested_format) {
        if (data)
            Free();
        data = stbi_load(path.c_str(), &width, &height, &channels, GetChannelCount(requested_format));
    }

    void Data::Free() {
        stbi_image_free(data);
        data = nullptr;
        width = 0;
        height = 0;
        channels = 0;
    }

    uint32_t Data::GetChannelCount(Format format) {
        static constexpr uint32_t S_FORMAT_TO_COUNT[] = {
            1, 2, 3, 4, // R  , RG  , RGB  , RGBA
            1, 2, 3, 4, // R8U, RG8U, RGB8U, RGBA8U
            1		    // R32
        };
        return S_FORMAT_TO_COUNT[static_cast<uint32_t>(format)];
    }
}