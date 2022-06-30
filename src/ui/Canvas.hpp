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
//		UIElementData(ushort x, ushort y, ubyte charCount, ubyte scale, ushort charOffset);
//		uint a, b;
//	};
//
//	class Textbox : public UIElement {
//	public:
//		Textbox(UICanvas& canvas, int x, int y, const std::string& text = "");
//		~Textbox();
//
//		void SetText(const std::string& text);
//		void SetScale(uint scale);
//
//	private:
//		UICanvas* owner;
//		uint index;
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