#pragma once

#include <unordered_map>
#include <string>

namespace zore {

    struct string_hash {
        using is_transparent = void;
        std::size_t operator()(const std::string& v) const noexcept {
            return std::hash<std::string>{}(v);
        }
        size_t operator()(std::string_view v) const noexcept {
            return std::hash<std::string_view>{}(v);
        }
    };

    template <typename T>
    using string_unordered_map = std::unordered_map<std::string, T, string_hash, std::equal_to<void>>;
}