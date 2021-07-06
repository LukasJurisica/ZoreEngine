#pragma once
#include "debug/Exception.hpp"
#include "win32_core.hpp"

namespace zore {

	//========================================================================
	//	Window Exception
	//========================================================================

	class WindowException : public Exception {
	public:
		WindowException(int line, const char* file, DWORD dw);
		~WindowException() = default;

	private:
		static std::string GetErrorString(DWORD dw);
	};
}

#define WND_ERROR(dw) zore::WindowException(__LINE__, __FILE__, dw)
#define WND_LAST_ERROR() zore::WindowException(__LINE__, __FILE__, GetLastError())