#pragma once
#include "zore/debug/exception.hpp"
#include "zore/platform/win32/win32_core.hpp"

namespace zore {

	//========================================================================
	//	Window Exception
	//========================================================================

	class WindowsException : public Exception {
	public:
		WindowsException(int line, const char* file, DWORD error_code);
		~WindowsException() = default;

		static std::string GetErrorString(DWORD error_code);
	};
}

#define WND_ERROR(error_code) zore::WindowsException(__LINE__, __FILE__, error_code)
#define WND_LAST_ERROR() zore::WindowsException(__LINE__, __FILE__, GetLastError())