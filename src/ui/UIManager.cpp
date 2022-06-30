//#include "ui/UIManager.hpp"
//#include "debug/Debug.hpp"
//
//namespace zore {
//
//	uint UIManager::RegisterTextbox(UICanvas* canvas, Textbox* textbox) {
//		canvas->elements.push_back(textbox);
//		canvas->elementData.push_back({0, 0, 0, 0, 0});
//		return static_cast<uint>(canvas->elements.size()) - 1;
//	}
//
//	void UIManager::UpdateTextboxText(UICanvas* canvas, uint index, const std::string& text) {
//		UIElementData& data = canvas->elementData[index];
//
//		uint charCount = static_cast<uint>(text.length());
//		uint scale = (data.b >> 16) & 0xFF;
//		uint charOffset = data.b & 0xFFFF;
//		
//		data.b = (charCount << 24) + (scale << 16) + charOffset;
//
//		for (int i = index; i < canvas->elementData.size(); i++) {
//
//		}
//	}
//}