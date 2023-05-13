#include "zore/ui/Text.hpp"
#include "zore/graphics/Buffer.hpp"
#include "zore/graphics/Texture.hpp"
#include "zore/debug/Debug.hpp"
#include <vector>

namespace zore {

	//========================================================================
	//  Platform Agnostic Font Class
	//========================================================================

	Font::Font(const std::string& name) {
		int asciiTextRange = 127 - 32;
		std::vector<std::string> textFilenames(asciiTextRange);
		for (int i = 0; i < asciiTextRange; i++)
			textFilenames[i] = std::to_string(i + 32) + ".png";
		texture.Set(textFilenames, "assets/fonts/" + name + "/", Texture::Format::RGB);
	}

	Texture2DArray& Font::GetTextureArray() {
		return texture;
	}

	//========================================================================
	//  Platform Agnostic Textbox Class
	//========================================================================

	struct UIElementData {//a((x << 16) + y), b((scale << 24) + (charCount << 16) + charOffset) {}
		UIElementData(uint16_t x, uint16_t y, uint8_t charCount, uint8_t scale, uint16_t charOffset) :
			x(x), y(y), offset(charOffset), count(charCount), scale(scale) {}
		uint16_t x, y, offset;
		uint8_t count, scale;
	};

	std::vector<Textbox*> textboxes;
	std::vector<UIElementData> elementData;
	std::string charData;
	VertexBuffer* elementBuffer;
	ShaderStorageBuffer* charBuffer;

	Textbox::Textbox(uint16_t x, uint16_t y, const std::string& text, bool flush) {
		index = static_cast<uint32_t>(textboxes.size());
		textboxes.push_back(this);

		// Create Element Entry
		DEBUG_ENSURE(text.length() < 256, "Cannot render string with length greater than 256");
		uint8_t charCount = static_cast<uint8_t>(text.length());
		uint16_t charOffset = static_cast<uint16_t>(charData.length());
		elementData.push_back({ x, y, charCount, 1, charOffset });
		charData += text;
		/* ------
		If this breaks, its caused by an incompatable endian pattern between the text.glsl shader
		and the way the buffer is constructed. Revisit how the data is packed
		------ */

		if (flush)
			Flush();
	}

	Textbox::~Textbox() {

	}

	void Textbox::Flush() {
		delete elementBuffer;
		elementBuffer = new VertexBuffer(elementData.data(), sizeof(UIElementData) * static_cast<uint32_t>(elementData.size()), sizeof(UIElementData), 1);
		delete charBuffer;
		charBuffer = new ShaderStorageBuffer(charData.c_str(), sizeof(uint8_t) * static_cast<uint32_t>(charData.length()));
		charBuffer->Bind(0);
	}

	uint32_t Textbox::Bind() {
		elementBuffer->Bind();
		return static_cast<uint32_t>(elementData.size());
	}

	void Textbox::SetText(const std::string& text, bool flush) {
		charData.replace(elementData[index].offset, elementData[index].count, text);
		elementData[index].count = static_cast<uint8_t>(text.length());
		for (int i = index; i < elementData.size() - 1; i++) {
			int nextIndex = i + 1;
			elementData[nextIndex].offset = elementData[i].offset + elementData[i].count;
		}
		if (flush)
			Flush();
	}
}