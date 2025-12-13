#pragma once
#include "zore/debug/exception.hpp"
#include "zore/debug/logger.hpp"

#define ENSURE(expr, msg) (expr || zore::Exception::AssertionException(__LINE__, __FILE__, msg))

#if defined(_DEBUG) || true
#define DEBUG_ENSURE(expr, msg) ENSURE(expr, msg)
#define DEBUG_ONLY(call) call
#define IS_DEBUG 1
#define IS_RELEASE 0
#else
#define DEBUG_ENSURE(expr, msg)
#define DEBUG_ONLY(call)
#define IS_DEBUG 0
#define IS_RELEASE 1
#endif