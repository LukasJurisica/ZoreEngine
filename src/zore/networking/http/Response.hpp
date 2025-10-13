#pragma once

#include "zore/networking/http/status.hpp"
#include <unordered_map>
#include <string>

namespace zore::net::http {

    class Response {
    public:
        Response(const std::string& response);
        ~Response() = default;

        Status GetStatus() const { return m_status; }
        int GetStatusClass() const { return static_cast<int>(m_status) / 100; }
        const std::string& GetField(const std::string& key) const;
        const std::string& GetBody() const { return m_body; }

    private:
        Status m_status;
        std::string m_body;
        std::unordered_map<std::string, std::string> m_fields;
    };
}