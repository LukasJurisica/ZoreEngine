#pragma once

#include "zore/math/simd/simd_core.hpp"

#if SIMD_SSE >= ENCODE_VERSION(2, 0, 0)
#include "zore/math/simd/sse/sse_float32_4.hpp"
#define SIMD_FLOAT32_4 true
#endif