#pragma once

#include "zore/networking/socket.hpp"
#include "zore/networking/http/request.hpp"
#include "zore/networking/http/response.hpp"

namespace zore::net::http {

    class Client {
    public:
        Client();
        Client(const std::string& host, uint16_t port);
		~Client() = default;

		bool Connect(const std::string& host, uint16_t port);
        Response Make(Request request);
		void Disconnect();

    private:
		Socket m_socket;
        std::string m_host;
    };
}