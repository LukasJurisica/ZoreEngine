#pragma once

// Taken from https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time
#if defined(__x86_64__) || defined(_M_X64)
#define ARCHITECTURE_x86_64
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define ARCHITECTURE_x86_32
#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__)
#define ARCHITECTURE_ARM
#endif

#if defined(_WIN32)
#define PLATFORM_WINDOWS
#elif defined(__unix__)
#define PLATFORM_LINUX
#endif

#if defined(_MSC_VER)
#define COMPILER_MSVC
#define ZORE_ALWAYS_INLINE __forceinline
#elif defined(__GNUG__)
#define COMPILER_GCC
#define ZORE_ALWAYS_INLINE __attribute__((always_inline))
#endif