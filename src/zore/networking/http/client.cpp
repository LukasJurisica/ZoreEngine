#include "zore/networking/http/client.hpp"
#include "zore/networking/secure_socket.hpp"
#include "zore/debug.hpp"
#include <array>

#define MAX_RESPONSE_SIZE 4096

namespace zore::net::http {

	Client::Client() : m_socket(nullptr), m_scheme(Scheme::HTTP), m_port(0), m_connection(Connection::CLOSE), m_timeout(0) {}

	Client::Client(const std::string& host, Scheme scheme, uint16_t port) : m_socket(nullptr), m_connection(Connection::CLOSE), m_timeout(0) {
		Connect(host, scheme, port);
	}

	void Client::SetTimeout(std::chrono::milliseconds timeout) {
		m_timeout = timeout;
		if (m_socket)
			m_socket->SetTimeout(timeout);
	}

	bool Client::Connect(const std::string& host, Scheme scheme, uint16_t port) {
		if (host.empty())
			return false;
		if (port == 0)
			port = (scheme == Scheme::HTTPS) ? 443 : 80;
		m_host = host;
		m_scheme = scheme;
		m_port = port;

		if (scheme == Scheme::HTTPS)
			m_socket.reset(new SecureSocket(Protocol::TCP, true));
		else
			m_socket.reset(new Socket(Protocol::TCP, true));
		m_socket->SetTimeout(m_timeout);

		Address address = Address::Resolve(host, port, Protocol::TCP);
		if (address.IsValid())
			if (m_socket->Connect(address) == Socket::Status::DONE)
				return true;
		Disconnect();
		return false;
	}

	Response Client::Make(Request request) {
		Response response(request.GetMethod());
		if (m_socket) {

			request.SetField("Host", std::format("{}:{}", m_host, m_port), false);
			request.SetField("Connection", m_connection == Connection::CLOSE ? "close" : "keep-alive");
			std::string req = request.Build();

			Socket::Result result = m_socket->Send(req.data(), req.size());
			if (result.status != Socket::Status::DONE) {
				Logger::Error("HTTP request failed: Failed to send request");
				return response;
			}

			std::array<char, MAX_RESPONSE_SIZE> buffer;
			while (true) {
				const Socket::Result result = m_socket->Receive(buffer.data(), MAX_RESPONSE_SIZE);

				if (result.status == Socket::Status::DONE) {
					if (response.Append(buffer.data(), result.bytes_transferred))
						return response;
				}
				else if (result.status == Socket::Status::DISCONNECTED) {
					break;
				}
				else if (result.status == Socket::Status::TIMED_OUT) {
					Logger::Error("HTTP request failed: Server timed out");
					return response;
				}
				else {
					Logger::Error("HTTP request failed: Failed to receive response");
					return response;
				}
			}
			return response;
		}
		Logger::Error("HTTP request failed: Socket not connected or unavailable");
		return response;
	}

	void Client::Disconnect() {
		m_socket.reset();
		m_host.clear();
	}
}
