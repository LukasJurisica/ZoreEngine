#pragma once

#include "zore/Platform.hpp"
#include <format>

#if defined(PLATFORM_WINDOWS)
#include "zore/platform/win32/win32_Exception.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#undef ERROR
#undef DELETE

#define SOCK_CONNECTION_REFUSED   WSAECONNREFUSED
#define SOCK_CONNECTION_RESET     WSAECONNRESET
#define SOCK_CONNECTION_ABORTED   WSAECONNABORTED
#define SOCK_CONNECTION_TIMED_OUT WSAETIMEDOUT
#define SOCK_HOST_UNREACHABLE     WSAEHOSTUNREACH
#define SOCK_NETWORK_UNREACHABLE  WSAENETUNREACH

#define SOCK_WOULD_BLOCK          WSAEWOULDBLOCK
#define SOCK_ALREADY              WSAEALREADY
#define SOCK_NET_RESET            WSAENETRESET
#define SOCK_NOT_CONNECTED        WSAENOTCONN
#define SOCK_IS_CONNECTED         WSAEISCONN

#elif defined(PLATFORM_LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

namespace zore::net {

	static inline std::string GetLastError(const std::string& function) {
#if defined(PLATFORM_WINDOWS)
		int error_code = WSAGetLastError();
		std::string error_message = zore::WindowsException::GetErrorString(error_code);
#elif defined(PLATFORM_LINUX)
		int error_code = errno;
		std::string error_message(' ', strerrorlen_s(error_code));
		strerror_s(error_message.data(), error_message.length(), error_code);
#endif
		return std::format("Socket error {} executing \"{}\": {}", error_code, function, error_message);
	}
}