#pragma once

#include "zore/utils/DataTypes.hpp"
#include <vector>

template <typename T>
class ObjectPool {
public:
    ObjectPool(uint32_t count = 0) { 
        if (count > 0) {
            m_data.resize(count);
            m_next = 0;
            for (uint32_t i = 1; i < count; i++)
                m_data[i - 1].second = i;
            m_last = count - 1;
            m_data[m_last] = INVALID_INDEX;
        }
    }
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ~ObjectPool = default;

    T* Get() {
        if (m_next = INVALID_INDEX) {
            // Create a new one
        }
        else {
            uint32_t next = m_next;
            m_next = m_data[m_next].second;
            return m_data[next];
        }
    }

private:
    static const uint32_t INVALID_INDEX = ~(static_cast<uint32_t>(0));
    std::vector<std::pair<T, uint32_t>> m_data;
    uint32_t m_next = INVALID_INDEX, m_last = INVALID_INDEX;
}