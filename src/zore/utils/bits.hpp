#pragma once

#define GET_BIT(num, bit) ((num >> bit) & 1)
#define FILL_BIT(num, bit) num |= (1UL << bit)
#define CLEAR_BIT(num, bit) num &= ~(1UL << bit)
#define TOGGLE_BIT(num, bit) num ^= (1UL << bit)
#define SET_BIT(num, val, bit) num = (num & ~(1UL << bit)) | (val << bit);

#define GET_N_BITS(num, cnt, off) ((num >> off) & ~(~0UL << cnt))
#define FILL_N_BITS(num, cnt, off) num |= ~(~0UL << cnt) << off
#define CLEAR_N_BITS(num, cnt, off) num &= ~(~(~0UL << cnt) << off)
#define TOGGLE_N_BITS(num, cnt, off) num ^= ~(~0UL << cnt) << off
#define SET_N_BITS(num, val, cnt, off) num = (num & ~(~(~0UL << cnt) << off)) | (val << off)

#define PACK_BITS(num, val, off) num |= ((val) << off)
#define UNPACK_BITS(num, msk, off) ((num >> off) & msk)