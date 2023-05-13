#pragma once
#include <stdexcept>
#include <string>

namespace zore {

	class Exception : public std::runtime_error {
	public:
		Exception(const std::string& type, const std::string& msg);
		Exception(const std::string& type, int line, const std::string& file, const std::string& msg);
		~Exception() = default;
		static bool AssertionException(int line, const std::string& file, const std::string& msg);
		const char* GetType() const;

	private:
		std::string type;
	};
}

#define ZORE_EXCEPTION(msg) zore::Exception("Zore Engine Exception", __LINE__, __FILE__, msg)