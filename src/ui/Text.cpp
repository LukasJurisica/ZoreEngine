#include "ui/Text.hpp"
#include "graphics/Buffer.hpp"
#include "graphics/Texture.hpp"
#include "debug/Debug.hpp"
#include <vector>

namespace zore {

	//========================================================================
	//  Platform Agnostic Font Class
	//========================================================================

	uint fontBindPoint;

	Font::Font(const std::string& name) {
		int asciiTextRange = 127 - 32;
		std::vector<std::string> textFilenames(asciiTextRange);
		for (int i = 0; i < asciiTextRange; i++)
			textFilenames[i] = std::to_string(i + 32) + ".png";
		texture = Texture2DArray::Create(textFilenames, "assets/fonts/" + name + "/", TextureFormat::RGB);
		Bind();
	}

	Font::~Font() {
		delete texture;
	}

	void Font::Bind() {
		texture->Bind(fontBindPoint);
	}

	void Font::SetTextureSlot(uint slot) {
		fontBindPoint = slot;
	}

	//========================================================================
	//  Platform Agnostic Textbox Class
	//========================================================================

	struct UIElementData {//a((x << 16) + y), b((scale << 24) + (charCount << 16) + charOffset) {}
		UIElementData(ushort x, ushort y, ubyte charCount, ubyte scale, ushort charOffset) :
			x(x), y(y), offset(charOffset), count(charCount), scale(scale) {}
		ushort x, y, offset;
		ubyte count, scale;
	};

	std::vector<Textbox*> textboxes;
	std::vector<UIElementData> elementData;
	std::string charData;
	InstanceArrayBuffer* elementBuffer;
	ShaderStorageBuffer* charBuffer;

	Textbox::Textbox(ushort x, ushort y, const std::string& text) {
		index = static_cast<uint>(textboxes.size());
		textboxes.push_back(this);

		// Create Element Entry
		DEBUG_ENSURE(text.length() < 256, "Cannot render string with length greater than 256");
		ubyte charCount = static_cast<ubyte>(text.length());
		ushort charOffset = static_cast<ushort>(charData.length());
		elementData.push_back({ x, y, charCount, 1, charOffset });
		charData += text;
		
		/* ------
		If this breaks, its caused by an incompatable endian pattern between the text.glsl shader
		and the way the buffer is constructed. Revisit how the data is packed
		------ */
	}

	Textbox::~Textbox() {

	}

	void Textbox::Flush() {
		delete elementBuffer;
		elementBuffer = InstanceArrayBuffer::Create(elementData.data(), sizeof(UIElementData) * static_cast<uint>(elementData.size()), sizeof(UIElementData));
		delete charBuffer;
		charBuffer = ShaderStorageBuffer::Create(charData.c_str(), sizeof(ubyte) * static_cast<uint>(charData.length()));
		charBuffer->Bind(0);
	}

	uint Textbox::Bind() {
		elementBuffer->Bind();
		return static_cast<uint>(elementData.size());
	}

	void Textbox::SetText(const std::string& text) {
		charData.replace(elementData[index].offset, elementData[index].count, text);
		elementData[index].count = static_cast<ubyte>(text.length());
		for (int i = index; i < elementData.size() - 1; i++) {
			int nextIndex = i + 1;
			elementData[nextIndex].offset = elementData[i].offset + elementData[i].count;
		}
	}
}