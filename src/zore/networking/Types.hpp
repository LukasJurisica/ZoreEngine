#pragma once

#include "zore/utils/DataTypes.hpp"
#include "zore/Platform.hpp"

#if defined(PLATFORM_WINDOWS)
typedef uintptr_t socket_t;
typedef int sockaddr_length_t;
#elif defined(PLATFORM_LINUX)
typedef int socket_t;
typedef int sockaddr_length_t;
#endif

struct sockaddr_storage;
struct sockaddr;

namespace zore::net {
	enum class Protocol { TCP, UDP };
}