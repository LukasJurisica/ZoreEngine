#include "zore/networking/Socket.hpp"
#include "zore/Platform.hpp"
#include "zore/Debug.hpp"

#if defined(PLATFORM_WINDOWS)
#include "zore/platform/win32/win32_Exception.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(PLATFORM_LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#endif

namespace zore {

	//========================================================================
	//	A simple utility class to initialize sockets on Windows
	//========================================================================

#if defined(PLATFORM_WINDOWS)
	class SocketInitializer {
	public:
		SocketInitializer() {
			WSADATA wsa_data;
			ENSURE(WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0, "Failed to initialize Winsock.");
			ENSURE(LOBYTE(wsa_data.wVersion) == 2 && HIBYTE(wsa_data.wVersion) == 2, "Version 2.2 of Winsock is not available.");
			Logger::Info("Winsock2 Initialization Complete.");
		}

		static void Initialize() {
			static SocketInitializer socket_initializer;
		}

		~SocketInitializer() {
			WSACleanup();
			Logger::Info("Winsock2 Cleanup Complete.");
		}
	};
#endif

	//========================================================================
	//	An abstract generic socket class
	//========================================================================

	Socket::Socket(const std::string& address, int port, Protocol protocol) : m_socket_id(-1) {
#ifdef PLATFORM_WINDOWS
			SocketInitializer::Initialize();
#endif
		
		int status;
		addrinfo hints;

		memset(&hints, 0, sizeof(addrinfo));
		hints.ai_family = AF_UNSPEC;
		static constexpr int PROTOCOL_TO_SOCKET_TYPE[] = { SOCK_STREAM, SOCK_DGRAM };
		hints.ai_socktype = PROTOCOL_TO_SOCKET_TYPE[static_cast<int>(protocol)];
		hints.ai_flags = AI_PASSIVE;

		// Get address info on the provided address
		if ((status = getaddrinfo(address == "" ? NULL : address.c_str(), std::to_string(port).c_str(), &hints, &m_results)) != 0) {
			Logger::Error("getaddrinfo error when connecting to " + address + "\n" + std::string(gai_strerror(status)));
			return;
		}

		// Initialize the socket file descriptor
		if ((m_socket_id = socket(m_results->ai_family, m_results->ai_socktype, m_results->ai_protocol)) == -1) {
			PrintLastError("socket");
			return;
		}
	}

	Socket::Socket(int socket_id, addrinfo* results) : m_socket_id(socket_id), m_results(results) {}

	Socket::~Socket() {
#if defined(PLATFORM_WINDOWS)
		closesocket(m_socket_id);
#elif defined(PLATFORM_LINUX)
		close(m_socket_id);
#endif
	}

	bool Socket::IsValid() {
		return m_socket_id != -1;
	}

	std::string Socket::GetHostName() {
#ifdef PLATFORM_WINDOWS
		SocketInitializer::Initialize();
#endif
		char buffer[256];
		if (gethostname(buffer, 256) == -1) {
			PrintLastError("gethostname");
			return "Failed to get hostname";
		}
		return std::string(buffer);
	}

	void Socket::PrintLastError(const std::string & function) {
#if defined(PLATFORM_WINDOWS)
		int error_code = WSAGetLastError();
		std::string error_message = WindowsException::GetErrorString(error_code);
#elif defined(PLATFORM_LINUX)
		int error_code = errno;
		std::string error_message(' ', strerrorlen_s(error_code));
		strerror_s(error_message.data(), error_message.length(), error_code);
#endif
		Logger::Error("Socket error " + std::to_string(error_code) + " executing \"" + function + "\": " + error_message);
	}


	//========================================================================
	//	An active socket, used for external connections
	//========================================================================

	static int s_host_port = 3490;

	ActiveSocket::ActiveSocket(const std::string& address, int port, Protocol protocol) : Socket(address.c_str(), port, protocol) {
		if (!m_results)
			return;
		// Connect to a remote port
		if (connect(m_socket_id, m_results->ai_addr, static_cast<int>(m_results->ai_addrlen)) == -1) {
			PrintLastError("connect");
			return;
		}
		Logger::Log("Connected");

		freeaddrinfo(m_results);
	}

	std::string ActiveSocket::GetPeerName() {
		sockaddr address;
		int address_length = sizeof(address);

		if (getpeername(m_socket_id, &address, &address_length) == -1) {
			PrintLastError("getpeername");
			return "Failed to get peername";
		}
		return "TODO: Get peername from sockaddr";
	}

	bool ActiveSocket::SendPacket(const void* data, int* length) {
		int total_sent = 0, sent = 0;
		const char* char_data = reinterpret_cast<const char*>(data);

		while (total_sent < *length) {
			sent = send(m_socket_id, char_data + total_sent, *length - total_sent, 0);
			if (sent == -1)
				break;
			total_sent += sent;
		}

		*length = total_sent;
		return (sent == -1) ? false : true;
	}

	bool ActiveSocket::ReceivePacket(void* buffer, int* length) {
		char* char_buffer = reinterpret_cast<char*>(buffer);
		*length = recv(m_socket_id, char_buffer, *length, 0);
		return (*length == -1) ? false : true;
	}

	//========================================================================
	//	A passive socket, used for listening for incoming connections
	//========================================================================

	PassiveSocket::PassiveSocket(int port, Protocol protocol) : Socket("", port, protocol) {
		if (!m_results)
			return;

		// Bind our socket to a port
		if (bind(m_socket_id, m_results->ai_addr, static_cast<int>(m_results->ai_addrlen)) == -1) {
			PrintLastError("bind");
			return;
		}

		// Listen for incoming connections on a port
		if (listen(m_socket_id, 10) == -1) {
			PrintLastError("listen");
			return;
		}
		Logger::Info("Socket listening on port: " + std::to_string(port));

		freeaddrinfo(m_results); // free the linked-list
	}

	void PassiveSocket::AcceptConnections(std::vector<ActiveSocket>& connections) {
		sockaddr_storage address_storage;
		socklen_t address_size = sizeof(address_storage);
		sockaddr* address_storage_ptr = reinterpret_cast<sockaddr*>(&address_storage);
		int new_socket_id = static_cast<int>(accept(m_socket_id, address_storage_ptr, &address_size));

		if (new_socket_id == -1)
			PrintLastError("accept");
		else
			connections.emplace_back(new_socket_id, nullptr);
	}
}