#include "zore/utils/sized_integer.hpp"
#include "zore/platform.hpp"

namespace zore::net {
    
    // Zore Application to Network Byte Order
    static constexpr inline  int16_t zton( int16_t value) { return value; }
    static constexpr inline uint16_t zton(uint16_t value) { return value; }
    static constexpr inline  int32_t zton( int32_t value) { return value; }
    static constexpr inline uint32_t zton(uint32_t value) { return value; }
    static constexpr inline  int64_t zton( int64_t value) { return value; }
    static constexpr inline uint64_t zton(uint64_t value) { return value; }

    // Network to Zore Application Byte Order
    static constexpr inline  int16_t ntoz( int16_t value) { return value; }
    static constexpr inline uint16_t ntoz(uint16_t value) { return value; }
    static constexpr inline  int32_t ntoz( int32_t value) { return value; }
    static constexpr inline uint32_t ntoz(uint32_t value) { return value; }
    static constexpr inline  int64_t ntoz( int64_t value) { return value; }
    static constexpr inline uint64_t ntoz(uint64_t value) { return value; }
}