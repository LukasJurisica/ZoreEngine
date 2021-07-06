#include "Exception.hpp"

namespace zore {

	Exception::Exception(const std::string& type, const std::string& msg) :
		runtime_error("[Error] " + msg), type(type) {}

	Exception::Exception(const std::string& type, int line, const std::string& file, const std::string& msg) :
		runtime_error("[File] " + file + " (Line " + std::to_string(line) + ")\n\n[Error] " + msg), type(type) {}

	const char* Exception::GetType() const {
		return type.c_str();
	}

	bool Exception::AssertionException(int line, const std::string& file, const std::string& msg) {
		throw Exception("Assertion Exception", line, file, msg);
		return true;
	}
}