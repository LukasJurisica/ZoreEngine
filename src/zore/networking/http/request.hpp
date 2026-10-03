#pragma once

#include "zore/networking/http/enums.hpp"
#include "zore/structures/string_unordered_map.hpp"
#include <string>

namespace zore::net::http {

    class Request {
    public:
        Request(Method method, const std::string& uri);
        ~Request() = default;

		Method GetMethod() const { return m_method; }
		void SetMethod(Method method) { m_method = method; }
        void SetField(const std::string& key, const std::string& value, bool overwrite = true);
        void SetBody(const std::string& body, ContentType content_type = ContentType::TEXT);
        std::string Build() const;

    private:
        zore::string_unordered_map<std::string> m_fields;
        std::string m_uri;
        std::string m_body;
        Method m_method;
    };
}