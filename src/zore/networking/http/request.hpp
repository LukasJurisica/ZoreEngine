#pragma once

#include <unordered_map>
#include <string>

namespace zore::net::http {

    class Request {
    public:
        enum class Method { GET, PUT, POST, DELETE, PATCH, HEAD, OPTIONS, TRACE, CONNECT };

    public:
        Request(Method method, const std::string& uri);
        ~Request() = default;

		Method GetMethod() const { return m_method; }
		void SetMethod(Method method) { m_method = method; }
        void SetField(const std::string& key, const std::string& value, bool overwrite = true);
        void SetBody(const std::string& body);
        std::string Build() const;

    private:
        std::unordered_map<std::string, std::string> m_fields;
		std::string m_uri;
        std::string m_body;
        Method m_method;
    };
}