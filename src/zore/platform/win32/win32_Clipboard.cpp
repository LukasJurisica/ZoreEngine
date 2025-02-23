#include <zore/ui/Clipboard.hpp>
#include <zore/platform/win32/win32_Core.hpp>
#include <zore/Debug.hpp>

namespace zore {

	std::string Clipboard::Get() {
		std::string result = "";
		if (IsClipboardFormatAvailable(CF_TEXT) && OpenClipboard(NULL)) {
			if (HANDLE handle = GetClipboardData(CF_TEXT)) {
				if (LPVOID data = GlobalLock(handle)) {
					result = std::string(reinterpret_cast<char*>(data));
					GlobalUnlock(handle);
				}
			}
			CloseClipboard();
		}
		return result;
	}

	void Clipboard::Set(const std::string& value) {
		const size_t length = value.length() + 1;
		if (HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE, length)) {
			if (LPVOID data = GlobalLock(handle)) {
				memcpy(data, value.c_str(), length);
				GlobalUnlock(handle);
				if (OpenClipboard(NULL)) {
					EmptyClipboard();
					SetClipboardData(CF_TEXT, handle);
					CloseClipboard();
				}
			}
			GlobalFree(handle);
		}
	}
}