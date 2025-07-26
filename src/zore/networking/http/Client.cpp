#include "zore/networking/http/Client.hpp"
#include "zore/networking/Core.hpp"
#include "zore/Debug.hpp"

#define MAX_RESPONSE_SIZE 1024

namespace zore::net::http {

	Client::Client() : m_socket(Protocol::TCP, true) {}

	Client::Client(const std::string& host, uint16_t port) : m_socket(Protocol::TCP, true) {
		m_socket.Connect(Address::Resolve(host, port, Protocol::TCP));
		m_host = host;
	}

	bool Client::Connect(const std::string& host, uint16_t port) {
		m_host = host;
		return m_socket.Connect(Address::Resolve(host, port, Protocol::TCP)) == Socket::Status::DONE;
	}

	Response Client::Make(Request request) {
		if (m_socket.GetStatus() == Socket::Status::READY) {

			request.SetField("Host", m_host, false);
			if (request.GetMethod() == Request::Method::POST)
				request.SetField("Content-Type", "application/x-www-form-urlencoded", false);
			std::string req = request.Build();

			if (m_socket.Send(req.c_str(), static_cast<uint32_t>(req.length())) != Socket::Status::DONE)
				Logger::Error(GetLastError("send"));

			std::string response;
			char buffer[MAX_RESPONSE_SIZE];
			uint32_t response_size;
			while (m_socket.Receive(buffer, MAX_RESPONSE_SIZE, response_size) == Socket::Status::DONE)
				response += std::string(buffer, response_size);
			return Response(response);
		}

		return Response("Failed to complete request");
	}

	void Client::Disconnect() {
		m_socket.Close();
		m_host.clear();
	}
} 
