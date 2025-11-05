#pragma once

#define ENCODE_VERSION(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

// Taken from https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time
#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__) || defined(__amd64) || defined(_M_AMD64)
#define ARCHITECTURE_x86_64
#define ARCHITECTURE "x86_64"
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(__i686__) || defined(_M_IX86)
#define ARCHITECTURE_x86_32
#define ARCHITECTURE "x86_32"
#elif defined(__aarch64__) || defined(__aarch64)|| defined(_M_ARM64)
#define ARCHITECTURE_ARM_64
#define ARCHITECTURE "ARM_64"
#elif defined(__arm__) || defined(_M_ARM)
#define ARCHITECTURE_ARM_32
#define ARCHITECTURE "ARM_32"
#endif

#if defined(_WIN32)
#define PLATFORM_WINDOWS
#define IS_PLATFORM_WINDOWS 1
#define IS_PLATFORM_LINUX 0
#define IS_PLATFORM_MACOS 0
#define PLATFORM "WINDOWS"
#elif defined(__APPLE__) && defined(__MACH__)
#define IS_PLATFORM_WINDOWS 0
#define IS_PLATFORM_LINUX 0
#define IS_PLATFORM_MACOS 1
#define PLATFORM_MACOS
#define PLATFORM "MACOS"
#elif defined(__unix__)
#define IS_PLATFORM_WINDOWS 0
#define IS_PLATFORM_LINUX 1
#define IS_PLATFORM_MACOS 0
#define PLATFORM_LINUX
#define PLATFORM "LINUX"
#endif

#if defined(_MSC_VER)
#define COMPILER_MSVC
#define COMPILER "MSVC"
#define ALWAYS_INLINE __forceinline
#elif defined(__GNUG__)
#define COMPILER_GCC
#define COMPILER "GCC"
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ENDIANNESS "LITTLE_ENDIAN"
#elif defined(PLATFORM_WINDOWS) || defined(ARCHITECTURE_x86_64) || defined(ARCHITECTURE_x86_32)
#define ENDIANNESS "LITTLE_ENDIAN"
#else
#define ENDIANNESS "BIG_ENDIAN"
#endif