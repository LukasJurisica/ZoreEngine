#include "zore/networking/http/response.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug.hpp"
#include <charconv>

namespace zore::net::http {

    Response::Response(Method request_method) : m_status(Status::INVALID_RESPONSE), m_request_method(request_method) {}

    bool Response::Append(const char* data, size_t size) {
        m_body.append(data, size);
		if (m_fields.empty())
			ParseHeader();
		
		bool is_complete = false;
        if (!m_fields.empty() && m_request_method == Method::HEAD)
            is_complete = true;
		else if (GetStatusClass() == 1 || m_status == Status::NO_CONTENT || m_status == Status::RESET_CONTENT || m_status == Status::NOT_MODIFIED)
			is_complete = true;
		else if (m_fields.find("content-length") != m_fields.end() && m_body.size() >= std::stoul(m_fields["content-length"]))
			is_complete = true;
		else if (GetField("transfer-encoding") == "chunked" && m_body.find("0\r\n\r\n") != std::string::npos)
			is_complete = true;
		return is_complete;
    }

    const std::string& Response::GetField(const std::string& key) const {
        auto it = m_fields.find(key);
        if (it != m_fields.end())
            return it->second;
		static const std::string EMPTY;
        return EMPTY;
    }

	void Response::ParseHeader() {
        size_t header_size = m_body.find("\r\n\r\n");
        if (header_size == std::string::npos)
            return;

		std::string_view header_content(m_body.begin(), m_body.begin() + header_size);
        std::vector<std::string_view> header_entries;
        String::SplitV(header_entries, header_content, "\r\n");

        std::vector<std::string_view> result;
        String::SplitV(result, header_entries[0], " ");

        int code = static_cast<int>(Status::INVALID_RESPONSE);
        std::from_chars(result[1].data(), result[1].data() + result[1].size(), code);
        m_status = static_cast<Status>(code);

        for (int i = 1; i < header_entries.size(); i++) {
            size_t space = header_entries[i].find(": ");
            m_fields[String::Lower(header_entries[i].substr(0, space))] = header_entries[i].substr(space + 2);
        }
        m_body = m_body.substr(header_size + 4);
	}
}
