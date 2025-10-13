#pragma once

#include "zore/math/simd/core.hpp"

#if SIMD_SSE >= ENCODE_VERSION(2, 0, 0)
#include "zore/math/simd/sse/sse_uint32_4.hpp"
#endif