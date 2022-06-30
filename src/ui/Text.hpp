#pragma once
#include "utils/DataTypes.hpp"
#include <string>

namespace zore {

	//========================================================================
	//  Platform Agnostic Font Class
	//========================================================================

	class Texture2DArray;

	class Font {
	public:
		Font(const std::string& name);
		~Font();

		void Bind();
		static void SetTextureSlot(uint slot);

	private:
		Texture2DArray* texture;
	};

	//========================================================================
	//  Platform Agnostic Textbox Class
	//========================================================================
	
	class Textbox {
	public:
		Textbox(ushort x, ushort y, const std::string& text);
		~Textbox();
		
		static void Flush();
		static uint Bind();

		void SetText(const std::string& text);

	private:
		int index;
	};
}