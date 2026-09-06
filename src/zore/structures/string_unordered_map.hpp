#pragma once

#include "zore/utils/string.hpp"
#include <unordered_map>

namespace zore {

    struct string_hash {
        using is_transparent = void;
        size_t operator()(std::string_view v) const noexcept {
            return static_cast<size_t>(zore::String::Hash(v));
        }
    };

    template <typename T>
    using string_unordered_map = std::unordered_map<std::string, T, string_hash, std::equal_to<void>>;
}