#include "zore/ui/Font.hpp"

#include <unordered_map>

namespace zore::UI {

	//========================================================================
	//	Font Class
	//========================================================================

	static std::unordered_map<std::string, Font> s_fonts;

	Font::Font(const std::string& path, Texture::Format format) {
		constexpr uint8_t ascii_char_count = '~' - ' ' + 1;
		std::vector<std::string> glyph_filenames(ascii_char_count);
		for (int i = 0; i < ascii_char_count; i++)
			glyph_filenames[i] = std::to_string(i + 32) + ".png";
		m_texture_array.Set(glyph_filenames, path, Texture::Format::RGB);
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