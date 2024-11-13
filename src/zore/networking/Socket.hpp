#pragma once

#include "zore/utils/Span.hpp"

#include <string>
#include <vector>

struct addrinfo;

namespace zore {

	enum class Protocol { TCP, UDP };

	//========================================================================
	//	An abstract generic socket class
	//========================================================================

	class Socket {
	public:
		static std::string GetHostName();
		static void PrintLastError(const std::string& function);

		bool IsValid();

	protected:
		Socket(const std::string& address, int port, Protocol protocol);
		Socket(int socket_id, addrinfo* results);
		virtual ~Socket();

	protected:
		int m_socket_id;
		addrinfo* m_results;
	};

	//========================================================================
	//	An active Stream socket, used for external connections
	//========================================================================

	class ActiveSocket : public Socket {
	public:
		ActiveSocket(const std::string& address, int port, Protocol protocol);
		ActiveSocket(int socket_id, addrinfo* results) : Socket(socket_id, results) {}
		~ActiveSocket() = default;

		std::string GetPeerName();
		bool SendPacket(const void* data, int* length);
		bool ReceivePacket(void* buffer, int* max_length);
	};

	//========================================================================
	//	A passive socket, used for listening for incoming connections
	//========================================================================

	class PassiveSocket : public Socket {
	public:
		PassiveSocket(int port, Protocol protocol);
		~PassiveSocket() = default;

		void AcceptConnections(std::vector<ActiveSocket>& connections);
	};
}