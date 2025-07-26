#include "zore/networking/http/Response.hpp"
#include "zore/utils/String.hpp"
#include "zore/Debug.hpp"

namespace zore::net::http {

    Response::Response(const std::string& response) : m_status(Status::INVALID_RESPONSE) {
		size_t header_size = response.find("\r\n\r\n");
        if (header_size == std::string::npos)
            return;
        std::vector<std::string> header;
        String::Split(header, response.substr(0, header_size), "\r\n");
        
        std::vector<std::string> result;
        String::Split(result, header[0], " ");
        m_status = static_cast<Status>(std::stoi(result[1]));

        for (int i = 1; i < header.size(); i++) {
            size_t space = header[i].find(": ");
            m_fields[String::Lower(header[i].substr(0, space))] = header[i].substr(space + 2);
        }
        m_body = response.substr(header_size + 4);
    }

    const std::string& Response::GetField(const std::string& key) const {
        auto it = m_fields.find(key);
        if (it != m_fields.end())
            return it->second;
        return "";
    }
}