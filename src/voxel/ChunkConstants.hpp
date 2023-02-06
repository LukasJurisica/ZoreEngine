#pragma once

namespace zore {

	inline constexpr int CHUNK_WIDTH = 64; // Chunk width and height must be powers of 2
	inline constexpr int CHUNK_HALF_WIDTH = CHUNK_WIDTH / 2;
	inline constexpr int CHUNK_HEIGHT = 256;
	inline constexpr int CHUNK_HSLICE = CHUNK_WIDTH * CHUNK_WIDTH;
	inline constexpr int CHUNK_VSLICE = CHUNK_WIDTH * CHUNK_HEIGHT;
	inline constexpr int CHUNK_VOLUME = CHUNK_HSLICE * CHUNK_HEIGHT;
	inline constexpr int CHUNK_WIDTH_BIT_DEPTH = 6; // This needs to be updated manually to reflect chunk width
	inline constexpr int CHUNK_HEIGHT_BIT_DEPTH = 8; // This needs to be updated manually to reflect chunk height
}