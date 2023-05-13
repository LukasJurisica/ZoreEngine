#pragma once
#include "zore/utils/DataTypes.hpp"
#include "zore/graphics/Texture.hpp"
#include <string>

namespace zore {

	//========================================================================
	//  Platform Agnostic Font Class
	//========================================================================

	class Texture2DArray;

	class Font {
	public:
		Font(const std::string& name);
		~Font() = default;
		Texture2DArray& GetTextureArray();

	private:
		Texture2DArray texture;
	};

	//========================================================================
	//  Platform Agnostic Textbox Class
	//========================================================================
	
	class Textbox {
	public:
		Textbox(uint16_t x, uint16_t y, const std::string& text, bool flush = true);
		~Textbox();
		void SetText(const std::string& text, bool flush = true);

		static uint32_t Bind();
		static void Flush();

	private:
		int index;
	};
}