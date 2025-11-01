#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/platform.hpp"

#if defined(PLATFORM_WINDOWS)
typedef uintptr_t socket_t;
typedef int sockaddr_length_t;
typedef int socklen_t;
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
typedef int socket_t;
typedef int sockaddr_length_t;
#endif

struct sockaddr_storage;
struct sockaddr;

namespace zore::net {
	enum class Protocol { TCP, UDP };
}