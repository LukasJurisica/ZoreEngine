#pragma once
#include <string>
#include <glm/glm.hpp>

namespace zore {

#define TOSTR std::to_string
#define VEC2TOSTR(v) std::to_string(v.x) + " " + std::to_string(v.y)
#define VEC3TOSTR(v) std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z)
	
	class Logger {
	public:
		static void Log(const std::string& s);
		static void Log(float v);
		static void Info(const std::string& s);
		static void Warn(const std::string& s);
		static void Error(const std::string& s);
	};
}