#pragma once

#include "zore/networking/socket.hpp"
#include "zore/networking/http/request.hpp"
#include "zore/networking/http/response.hpp"
#include "zore/utils/memory.hpp"

namespace zore::net::http {

    enum class Scheme : uint8_t { HTTP, HTTPS };
	enum class Connection : uint8_t { CLOSE, KEEP_ALIVE };

    class Client {
    public:
        Client();
        Client(const std::string& host, Scheme scheme = Scheme::HTTP, uint16_t port = 0);
		~Client() = default;

		void SetConnection(Connection connection) { m_connection = connection; }
        void SetTimeout(std::chrono::milliseconds timeout);
		bool Connect(const std::string& host, Scheme scheme = Scheme::HTTP, uint16_t port = 0);
        Response Make(Request request);
		void Disconnect();

    private:
        zore::unique_pointer<Socket> m_socket;
        std::string m_host;
        uint16_t m_port;
        Scheme m_scheme;
        Connection m_connection;
		std::chrono::milliseconds m_timeout;
    };
}
