#pragma once

#include <zore/ui/Console.hpp>
#include <format>
#include <sstream>

namespace zore {

#define TOSTR std::to_string
#define VEC2TOSTR(v) std::to_string(v.x) + " " + std::to_string(v.y)
#define VEC3TOSTR(v) std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z)
#define VEC4TOSTR(v) std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z) + " " + std::to_string(v.w)

	class Logger {
	public:
		template<typename... Args>
		static void Log(Args... args) {
			std::stringstream result;
			Append(result, args...);
			Console::Print(result.str(), Console::LogLevel::LOG);
		}

		template<typename... Args>
		static void Info(Args... args) {
			std::stringstream result;
			Append(result, args...);
			Console::Print(result.str(), Console::LogLevel::INFO);
		}

		template<typename... Args>
		static void Warn(Args... args) {
			std::stringstream result;
			Append(result, args...);
			Console::Print(result.str(), Console::LogLevel::WARN);
		}

		template<typename... Args>
		static void Error(Args... args) {
			std::stringstream result;
			Append(result, args...);
			Console::Print(result.str(), Console::LogLevel::ERR);
		}

	private:
		template <typename T>
		inline static void Append(std::stringstream& result, const T& t) {
			result << t;
		};

		template <typename T, typename... Args>
		inline static void Append(std::stringstream& result, const T& t, Args... args) {
			Append(result, t);
			result << " ";
			Append(result, args...);
		};
	};
}