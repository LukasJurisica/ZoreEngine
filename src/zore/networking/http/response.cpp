#include "zore/networking/http/response.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug.hpp"
#include <charconv>

namespace zore::net::http {

    Response::Response(const std::string& response) : m_status(Status::INVALID_RESPONSE) {
		size_t header_size = response.find("\r\n\r\n");
        if (header_size == std::string::npos)
            return;
        std::vector<std::string_view> header;
        String::SplitV(header, std::string_view(response.begin(), response.begin() + header_size), "\r\n");

        std::vector<std::string_view> result;
        String::SplitV(result, header[0], " ");

        int code = static_cast<int>(Status::INVALID_RESPONSE);
        std::from_chars(result[1].data(), result[1].data() + result[1].size(), code);
        m_status = static_cast<Status>(code);

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