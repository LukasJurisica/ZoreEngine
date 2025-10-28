#pragma once

#include "zore/math/simd/simd_core.hpp"

#if SIMD_SSE >= ENCODE_VERSION(2, 0, 0)
#include "zore/math/simd/sse/sse_int32_4.hpp"
#define SIMD_INT32_4 true
#endif