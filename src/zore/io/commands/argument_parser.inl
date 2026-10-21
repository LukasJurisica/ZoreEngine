#pragma once

#ifdef __INTELLISENSE__
#include "zore/io/commands/argument_parser.hpp"
#endif
#include "zore/utils/String.hpp"
#include <stdexcept>
#include <charconv>

namespace zore::Command {

    //========================================================================
    //	Argument Parser
    //========================================================================

    template <numeric T>
    struct ArgumentParser<T> {
        static T Parse(StringReader& reader) {
            std::string_view token = reader.Read();
            T result{};
            const auto parsed = std::from_chars(token.data(), token.data() + token.size(), result);
            if (parsed.ec != std::errc{} || parsed.ptr != token.data() + token.size())
                throw ParseException("Expected a representable number", reader.GetPosition());
            return result;
        }
    };

    template <>
    struct ArgumentParser<std::string> {
        static std::string_view Parse(StringReader& reader) {
            std::string_view token = reader.Read();
            if (token == "")
                throw ParseException("Expected text", reader.GetPosition());
            return token;
        }
    };

    template <>
    struct ArgumentParser<bool> {
        static bool Parse(StringReader& reader) {
            std::string token = String::Lower(reader.Read());
            if (token == "true" || token == "1")
                return true;
            else if (token == "false" || token == "0")
                return false;
            throw ParseException("Expected true/1 or false/0", reader.GetPosition());
        }
    };

    template <>
    struct ArgumentParser<greedy_string> {
        static std::string_view Parse(StringReader& reader) {
            std::string_view result = reader.Remaining();
            if (result == "")
                throw ParseException("Expected text", reader.GetPosition());
            return result;
        }
    };
}