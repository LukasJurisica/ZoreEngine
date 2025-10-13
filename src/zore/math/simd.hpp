#pragma once

#include "zore/math/simd/simd_float32_4.hpp"
#include "zore/math/simd/simd_int32_4.hpp"
#include "zore/math/simd/simd_uint32_4.hpp"

namespace zm::simd {
#if SIMD_SSE >= ENCODE_VERSION(2, 0, 0)
	typedef float32_4 float32_m;
	typedef int32_4 int32_m;
	typedef uint32_4 uint32_m;
#endif
}