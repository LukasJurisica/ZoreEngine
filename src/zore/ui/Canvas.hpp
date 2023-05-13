//#pragma once
//#include "utils/DataTypes.hpp"
//#include "graphics/Buffer.hpp"
//#include <string>
//#include <vector>
//
//namespace zore {
//
//	class UIElement {};
//	class UICanvas;
//
//	struct UIElementData {
//		UIElementData(uint16_t x, uint16_t y, uint8_t charCount, uint8_t scale, uint16_t charOffset);
//		uint32_t a, b;
//	};
//
//	class Textbox : public UIElement {
//	public:
//		Textbox(UICanvas& canvas, int x, int y, const std::string& text = "");
//		~Textbox();
//
//		void SetText(const std::string& text);
//		void SetScale(uint32_t scale);
//
//	private:
//		UICanvas* owner;
//		uint32_t index;
//	};
//	
//	class UICanvas {
//		friend class UIManager;
//	public:
//		UICanvas();
//		~UICanvas();
//
//	private:
//		std::vector<UIElementData> elementData;
//		std::vector<UIElement*> elements;
//		InstanceArrayBuffer* elementBuffer;
//		ShaderStorageBuffer* charBuffer;
//	};
//
//}