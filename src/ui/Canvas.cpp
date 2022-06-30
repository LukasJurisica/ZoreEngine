//#include "ui/UIManager.hpp"
//#include "debug/Debug.hpp"
//
//namespace zore {
//
//	UICanvas::UICanvas() : elementBuffer(nullptr), charBuffer(nullptr) {}
//
//	UICanvas::~UICanvas() {
//		delete elementBuffer;
//		delete charBuffer;
//	}
//
//	UIElementData::UIElementData(ushort x, ushort y, ubyte charCount, ubyte scale, ushort charOffset) :
//		a((x << 16) + y), b((charCount << 24) + (scale << 16) + charOffset) {}
//
//	Textbox::Textbox(UICanvas& canvas, int x, int y, const std::string& text) : owner(&canvas) {
//		index = UIManager::RegisterTextbox(owner, this);
//	}
//}