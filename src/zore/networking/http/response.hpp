#pragma once

#include "zore/networking/http/enums.hpp"
#include "zore/structures/string_unordered_map.hpp"
#include <string>

namespace zore::net::http {

    class Response {
    public:
        Response(Method request_method);
        ~Response() = default;

		bool Append(const char* data, size_t size);
        Status GetStatus() const { return m_status; }
        int GetStatusClass() const { return static_cast<int>(m_status) / 100; }
        const std::string& GetField(const std::string& key) const;
        const std::string& GetBody() const { return m_body; }

    private:
        void ParseHeader();

    private:
        Status m_status;
        Method m_request_method;
        std::string m_body;
        zore::string_unordered_map<std::string> m_fields;
    };
}
