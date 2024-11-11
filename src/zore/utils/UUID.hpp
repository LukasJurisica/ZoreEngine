#pragma once

#include "zore/utils/DataTypes.hpp"
#include <atomic>

namespace zore {

    //========================================================================
	//	Local Unique Identifier Class
    //========================================================================

    class luid {
    public:
        static inline luid Generate() {
            static std::atomic<uint32_t> id = 0;
            return luid(++id);
        }
        operator uint32_t& () { return m_id; }
        operator uint32_t() const { return m_id; }

    private:
        luid(uint32_t id) : m_id(id) {}

    public:
        static inline constexpr uint32_t INVALID_ID = 0;

    private:
        uint32_t m_id;
    };

    //========================================================================
	//	Wide Unique Identifier Class
    //========================================================================

    class wuid {
    public:
        static wuid Generate() {
            static std::atomic<uint64_t> id = 0;
            return wuid(++id);
        }
        operator uint64_t& () { return m_id; }
        operator uint64_t() const { return m_id; }

    private:
        wuid(uint64_t id) : m_id(id) {}

    public:
        static inline constexpr uint64_t INVALID_ID = 0;

    private:
        uint64_t m_id;
    };

    //========================================================================
	//	Global Unique Identifier Class
    //========================================================================

	// TODO: Implement global unique identifier class

    class guid {
    public:
        static guid Generate() {

        }
    };

    typedef luid uuid_32;
    typedef wuid uuid_64;
}