#pragma once
#include "zore/debug/Exception.hpp"
#include "zore/debug/Logger.hpp"

#define PAUSE system("pause")
#define ENSURE(expr, msg) (expr || zore::Exception::AssertionException(__LINE__, __FILE__, #expr "\n" msg))

#ifdef _DEBUG
#define DEBUG_ENSURE(expr, msg) ENSURE(expr, msg)
#define DEBUG_ONLY(call) call
#define IS_DEBUG 1
#else
#define DEBUG_ENSURE(expr, msg)
#define DEBUG_ONLY(call)
#define IS_DEBUG 0
#endif