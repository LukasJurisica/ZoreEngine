#pragma once

#include <zore/ui/console.hpp>
#include <format>
#include <sstream>
#include <concepts>

template<typename T>
concept Loggable = requires(std::ostream & os, const T & value) {
	{ os << value } -> std::same_as<std::ostream&>;
};

namespace zore {

#define TOSTR std::to_string
#define VEC2TOSTR(v) std::to_string(v.x) + " " + std::to_string(v.y)
#define VEC3TOSTR(v) std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z)
#define VEC4TOSTR(v) std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z) + " " + std::to_string(v.w)

	class Logger {
	public:
		template<Loggable... Args>
		static void Log(Args... args) {
			std::stringstream result;
			((result << args << ' '), ...);
			Console::Print(result.str(), Console::LogLevel::LOG);
		}

		template<Loggable... Args>
		static void Info(Args... args) {
			std::stringstream result;
			((result << args << ' '), ...);
			Console::Print(result.str(), Console::LogLevel::INFO);
		}

		template<Loggable... Args>
		static void Warn(Args... args) {
			std::stringstream result;
			((result << args << ' '), ...);
			Console::Print(result.str(), Console::LogLevel::WARN);
		}

		template<Loggable... Args>
		static void Error(Args... args) {
			std::stringstream result;
			((result << args << ' '), ...);
			Console::Print(result.str(), Console::LogLevel::ERR);
		}

	private:
		template <Loggable T>
		inline static void Append(std::stringstream& result, const T& t) {
			result << t;
		};

		template <Loggable T, Loggable... Args>
		inline static void Append(std::stringstream& result, const T& t, Args... args) {
			Append(result, t);
			result << " ";
			Append(result, args...);
		};
	};
}