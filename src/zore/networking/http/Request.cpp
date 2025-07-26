#include "zore/networking/http/Request.hpp"
#include "zore/utils/String.hpp"
#include <format>

namespace zore::net::http {

	Request::Request(Method method, const std::string& uri) : m_uri(uri), m_method(method) {
		SetField("User-Agent", "ZoreEngine/1.0");
		SetField("Accept", "*/*");
		SetField("Connection", "close");
	}

	void Request::SetField(const std::string& key, const std::string& value, bool overwrite) {
		std::string k = String::Trim(String::Lower(key));
		if (m_fields.find(k) == m_fields.end() || overwrite)
			m_fields[k] = value;
	}

	void Request::SetBody(const std::string& body) {
		m_body = body;
	}

	std::string Request::Build() const {
		static std::string HTTP_METHOD_TO_STRING[] = { "GET", "PUT", "POST", "DELETE", "PATCH", "HEAD", "OPTIONS", "TRACE", "CONNECT" };
		std::string method_string = HTTP_METHOD_TO_STRING[static_cast<int>(m_method)];
		std::string request = std::format("{} {} HTTP/1.1\r\n", method_string, m_uri);
		for (const auto& field : m_fields)
			request += std::format("{}: {}\r\n", field.first, field.second);
		return request + "\r\n";
	}
}