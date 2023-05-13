#include "zore/platform/win32/win32_Exception.hpp"

namespace zore {

    //========================================================================
    //	Window Exception
    //========================================================================

    WindowException::WindowException(int line, const char* file, DWORD dw) :
        Exception("Window Exception", line, file, GetErrorString(dw)) {
    }

    std::string WindowException::GetErrorString(DWORD dw) {
        char* msgBuf = nullptr;
        size_t msgLen = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&msgBuf), 0, nullptr
        );
        if (msgLen == 0)
            return "Unidentified Error Code";
        std::string msg(msgBuf, msgLen);
        LocalFree(msgBuf);
        return msg;
    }
}