#pragma once

#include "zore/utils/sized_integer.hpp"

namespace zore {

    template<typename T>
    class uuid {
    public:
        uuid();
        uuid(const uuid&) = delete;
        uuid(uuid&& other) noexcept;
        uuid& operator=(const uuid&) = delete;
        uuid& operator=(uuid&& other) noexcept;
        ~uuid() = default;

        operator T&() { return m_id; }
        operator T() const { return m_id; }

    public:
        static inline constexpr T INVALID_ID = 0;

    private:
        T m_id;
    };

    typedef uuid<uint32_t> uuid_32;
    typedef uuid<uint64_t> uuid_64;
}

#include "zore/utils/uuid.inl"