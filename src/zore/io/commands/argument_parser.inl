#pragma once

#ifdef __INTELLISENSE__
#include "zore/io/commands/argument_parser.hpp"
#endif
#include "zore/io/commands/parse_exception.hpp"
#include "zore/utils/String.hpp"
#include <charconv>

namespace zore::Command {

    //========================================================================
    //	Argument Parser
    //========================================================================

    template <numeric T>
    struct ArgumentParser<T> {
        static T Parse(std::string_view token) {
            T result{};
            const auto parsed = std::from_chars(token.data(), token.data() + token.size(), result);
            if (parsed.ec != std::errc{} || parsed.ptr != token.data() + token.size())
                throw ParseException("Expected a representable number");
            return result;
        }
    };

    template <>
    struct ArgumentParser<std::string_view> {
        static std::string_view Parse(std::string_view token) {
            return token;
        }
    };

    template <>
    struct ArgumentParser<std::string> {
        static std::string Parse(std::string_view token) {
            return std::string(token);
        }
    };

    template <>
    struct ArgumentParser<bool> {
        static bool Parse(std::string_view token) {
            std::string lower = String::Lower(token);
            if (lower == "true" || lower == "1")
                return true;
            else if (lower == "false" || lower == "0")
                return false;
            throw ParseException("Expected true/1 or false/0, got: " + lower);
        }
    };
}