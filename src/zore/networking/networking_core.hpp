#pragma once

#include "zore/platform.hpp"

#if defined(PLATFORM_WINDOWS)
#include "zore/platform/win32/win32_exception.hpp"
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
#include <sys/time.h>
#include <netdb.h>
#include <errno.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCK_CONNECTION_REFUSED   ECONNREFUSED
#define SOCK_CONNECTION_RESET     ECONNRESET
#define SOCK_CONNECTION_ABORTED   ECONNABORTED
#define SOCK_CONNECTION_TIMED_OUT ETIMEDOUT
#define SOCK_HOST_UNREACHABLE     EHOSTUNREACH
#define SOCK_NETWORK_UNREACHABLE  ENETUNREACH
#define SOCK_WOULD_BLOCK          EWOULDBLOCK
#define SOCK_ALREADY              EALREADY
#define SOCK_NET_RESET            ENETRESET
#define SOCK_NOT_CONNECTED        ENOTCONN
#define SOCK_IS_CONNECTED         EISCONN
#endif