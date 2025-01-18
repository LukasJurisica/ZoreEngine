#include "zore/ui/Font.hpp"

#include <unordered_map>
#include <fstream>
#include <zore/Debug.hpp>

namespace zore::UI {

	//========================================================================
	//	Font Class
	//========================================================================

	static std::unordered_map<std::string, Font> s_fonts;

	Font::Font(const std::string& path, Texture::Format format) {
		if (path.rfind(".zbt") != std::string::npos) {
			uint32_t width, height, channels, glyph_count;

			// Initialize Tileset
			std::ifstream file(path, std::ios::binary);
			ENSURE(file.is_open(), "Unable to load font: " + path + ".");

			file.read(reinterpret_cast<char*>(&width), sizeof(uint32_t));
			file.read(reinterpret_cast<char*>(&height), sizeof(uint32_t));
			file.read(reinterpret_cast<char*>(&channels), sizeof(uint32_t));
			file.read(reinterpret_cast<char*>(&glyph_count), sizeof(uint32_t));

			uint32_t texture_size = width * height * channels * glyph_count;
			uint8_t* data = new uint8_t[texture_size];
			file.read(reinterpret_cast<char*>(data), texture_size);
			m_texture_array.Set(data, width, height, glyph_count, format);
		}
		else {
			constexpr uint8_t ascii_char_count = '~' - ' ' + 1;
			std::vector<std::string> glyph_filenames(ascii_char_count);
			for (int i = 0; i < ascii_char_count; i++)
				glyph_filenames[i] = std::to_string(i + 32) + ".png";
			m_texture_array.Set(glyph_filenames, path, format);
		}
	}

	Font& Font::Create(const std::string& name, const std::string& path, Texture::Format format) {
		return s_fonts.emplace(name, std::move(Font(path, format))).first->second;
	}

	Font& Font::Create(const std::string& path, Texture::Format format) {
		return Create(path.substr(path.find_last_of("/")), path, format);
	}

	Font* Font::Get(const std::string& name) {
		auto iter = s_fonts.find(name);
		if (iter != s_fonts.end())
			return &iter->second;
		return nullptr;
	}
}