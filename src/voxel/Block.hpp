#pragma once
#include "utils/DataTypes.hpp"

namespace zore {

	typedef ushort block_t;

	// Masks and Identifying Bits
	static constexpr block_t BLOCK_BIT   = 1 << 15;
	static constexpr block_t FLUID_BIT   = 1 << 14;
	static constexpr block_t OPAQUE_BIT  = 1 << 14;
	static constexpr block_t OPAQUE_MASK = BLOCK_BIT | OPAQUE_BIT;
	static constexpr block_t ID_MASK     = 65535u >> 2;

	// Blocks
	static constexpr block_t BLOCK_DIRT  = 0 | BLOCK_BIT | OPAQUE_BIT;
	static constexpr block_t BLOCK_GRASS = 1 | BLOCK_BIT | OPAQUE_BIT;
	static constexpr block_t BLOCK_STONE = 2 | BLOCK_BIT | OPAQUE_BIT;
	static constexpr block_t BLOCK_CLAY  = 3 | BLOCK_BIT | OPAQUE_BIT;
	static constexpr block_t BLOCK_SNOW  = 4 | BLOCK_BIT | OPAQUE_BIT;
	static constexpr block_t BLOCK_SAND  = 5 | BLOCK_BIT | OPAQUE_BIT;

	// Fluids
	static constexpr block_t FLUID_WATER = 0 | FLUID_BIT;
	static constexpr block_t FLUID_LAVA  = 1 | FLUID_BIT;

	// Sprites and Other
	static constexpr block_t BLOCK_AIR       = 0;
	static constexpr block_t SPRITE_PLANT    = 1;
	static constexpr block_t SPRITE_MUSHROOM = 2;
}