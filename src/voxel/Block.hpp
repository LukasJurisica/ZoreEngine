#pragma once

namespace zore {
	// Masks and Identifying Bits
	static constexpr ushort BLOCK_BIT   = 1 << 15;
	static constexpr ushort FLUID_BIT   = 1 << 14;
	static constexpr ushort OPAQUE_BIT  = 1 << 14;
	static constexpr ushort OPAQUE_MASK = BLOCK_BIT | OPAQUE_BIT;
	static constexpr uint   ID_MASK     = 65535u >> 2;

	// Blocks
	static constexpr ushort BLOCK_DIRT  = 0 | BLOCK_BIT | OPAQUE_BIT;
	static constexpr ushort BLOCK_GRASS = 1 | BLOCK_BIT | OPAQUE_BIT;
	static constexpr ushort BLOCK_STONE = 2 | BLOCK_BIT | OPAQUE_BIT;
	static constexpr ushort BLOCK_CLAY  = 3 | BLOCK_BIT | OPAQUE_BIT;

	// Fluids
	static constexpr ushort FLUID_WATER = 0 | FLUID_BIT;
	static constexpr ushort FLUID_LAVA  = 1 | FLUID_BIT;

	// Sprites and Other
	static constexpr ushort BLOCK_AIR    = 0;
	static constexpr ushort SPRITE_PLANT = 1;
}