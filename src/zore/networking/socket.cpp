#include "zore/networking/socket.hpp"
#include "zore/networking/networking_core.hpp"
#include "zore/debug.hpp"
#include <sys/types.h>

namespace zore::net {

	//========================================================================
	//	Base Socket
	//========================================================================

	AbstractSocket::AbstractSocket(socket_t socket_id, bool blocking) : m_socket_id(socket_id), m_blocking(blocking) {}

	AbstractSocket::~AbstractSocket() {
		Close();
	}

	void AbstractSocket::SetBlocking(bool blocking) {
		m_blocking = blocking;
		if (m_socket_id == INVALID_SOCKET)
			return;

#if defined(PLATFORM_WINDOWS)
		u_long mode = (blocking) ? 0 : 1;
		if (ioctlsocket(m_socket_id, FIONBIO, &mode) == SOCKET_ERROR)
			Logger::Error(GetLastError("ioctlsocket"));
#else
		int flags = fcntl(m_socket_id, F_GETFL);
		if (flags == -1) {
			Logger::Error(GetLastError("fcntl F_GETFL"));
			return;
		}
		(blocking) ? (flags &= ~O_NONBLOCK) : (flags |= O_NONBLOCK);
		if (fcntl(m_socket_id, F_SETFL, flags) == -1) {
			Logger::Error(GetLastError("fcntl F_SETFL"));
			return;
		}
#endif
	}

	void AbstractSocket::Close() {
		if (m_socket_id != INVALID_SOCKET) {
#if defined(PLATFORM_WINDOWS)
			closesocket(m_socket_id);
#else
			close(m_socket_id);
#endif
			m_socket_id = INVALID_SOCKET;
		}
	}

	//========================================================================
	//	Connection Socket
	//========================================================================

	static constexpr int SOCKET_TYPE_MAP[] = { SOCK_STREAM, SOCK_DGRAM };
	static constexpr int SOCKET_PROTOCOL_MAP[] = { IPPROTO_TCP, IPPROTO_UDP };

    Socket::Socket(Protocol protocol, bool blocking) : AbstractSocket(INVALID_SOCKET, blocking) {
		m_socket_type = SOCKET_TYPE_MAP[static_cast<int>(protocol)];
		m_socket_protocol = SOCKET_PROTOCOL_MAP[static_cast<int>(protocol)];
        m_sequence_id = 0;
		SetBlocking(blocking);
    }

	Socket::Socket(const Address& address, Protocol protocol, bool blocking) : AbstractSocket(INVALID_SOCKET, blocking) {
		m_socket_type = SOCKET_TYPE_MAP[static_cast<int>(protocol)];
		m_socket_protocol = SOCKET_PROTOCOL_MAP[static_cast<int>(protocol)];
        m_sequence_id = 0;
        Connect(address);
	}

	Socket::Socket(socket_t socket_id, Protocol protocol, bool blocking) : AbstractSocket(socket_id, blocking) {
		m_socket_type = SOCKET_TYPE_MAP[static_cast<int>(protocol)];
		m_socket_protocol = SOCKET_PROTOCOL_MAP[static_cast<int>(protocol)];
        m_sequence_id = 0;
		SetBlocking(blocking);
	}
	
	Socket::~Socket() {
		Close();
	}


	Address Socket::GetSelfAddress() const {
		sockaddr_in address;
		socklen_t address_size = sizeof(address);
		if (getsockname(m_socket_id, reinterpret_cast<sockaddr*>(&address), &address_size) == SOCKET_ERROR) {
			Logger::Error(GetLastError("getsockname"));
			return Address(nullptr);
		}
		return Address(ntohl(address.sin_addr.s_addr), ntohs(address.sin_port));
	}

	Address Socket::GetPeerAddress() const {
		sockaddr_in address;
		socklen_t address_size = sizeof(address);
		if (getpeername(m_socket_id, reinterpret_cast<sockaddr*>(&address), &address_size) == SOCKET_ERROR) {
			Logger::Error(GetLastError("getpeername"));
			return Address(nullptr);
		}
		return Address(ntohl(address.sin_addr.s_addr), ntohs(address.sin_port));
	}

	Socket::Status Socket::Connect(const Address& address) {
		if (address.IsValid() == false) {
			Logger::Error("Unable to connect socket: Invalid address provided");
			return Status::ERROR;
		}
		Close();
        m_sequence_id = 0;
		if ((m_socket_id = socket(address.GetFamily(), m_socket_type, m_socket_protocol)) == INVALID_SOCKET) {
			Logger::Error(GetLastError("socket"));
			return Status::ERROR;
		}
		if (connect(m_socket_id, address.GetSockAddress(), address.GetSockAddressSize()) == SOCKET_ERROR) {
			Logger::Error(GetLastError("connect"));
			return Status::ERROR;
		}
		SetBlocking(m_blocking);
		Logger::Info("Socket connected to: " + std::string(address));
		return Status::DONE;
	}

	Socket::Status Socket::Send(Packet& packet) {
		if (m_socket_id == INVALID_SOCKET)
			return Status::DISCONNECTED;
        packet.SetHeader(0, m_sequence_id++, 1, 0);
        return Send(packet.Data(), packet.Size());
	}

    Socket::Status Socket::Send(const void* data, uint32_t size) {
		if (m_socket_id == INVALID_SOCKET)
			return Status::DISCONNECTED;
		if (!data || size == 0)
			return Status::ERROR;

		uint32_t sent = 0;
        while (sent < size) {
            int result = send(m_socket_id, static_cast<const char*>(data) + sent, size - sent, 0);
            if (result < 0) {
                Logger::Error(GetLastError("send"));
                return Status::ERROR;
            }
            sent += result;
        }
		return Status::DONE;
    }

	Socket::Status Socket::Receive(Packet& packet) {
		if (m_socket_id == INVALID_SOCKET)
			return Status::DISCONNECTED;
        packet.Clear();
		uint32_t recieved;
		Status status = Receive(packet.Header(), packet.HeaderSize(), recieved);
        if (status != Status::DONE || recieved != packet.HeaderSize())
            return Status::ERROR;
        uint16_t* header = packet.ParseHeader();
        packet.Resize(header[2]);
		return Receive(packet.Payload(), packet.PayloadSize(), recieved);
	}

    Socket::Status Socket::Receive(void* data, uint32_t size, uint32_t& recieved) {
		recieved = 0;
		if (m_socket_id == INVALID_SOCKET)
			return Status::DISCONNECTED;
		if (!data || size == 0)
			return Status::ERROR;

		int result = recv(m_socket_id, static_cast<char*>(data) + recieved, size - recieved, 0);
		if (result == 0)
			return Status::DISCONNECTED;
        else if (result < 0)
            return Status::ERROR;
        recieved += result;
        return Status::DONE;
    }

    Socket::Status Socket::GetStatus() const {
        if (m_socket_id == INVALID_SOCKET)
            return Status::DISCONNECTED;
        return Status::READY;
    }

	//========================================================================
	//	Listener Socket
	//========================================================================

	Listener::Listener(bool blocking) : AbstractSocket(INVALID_SOCKET, blocking) {}

	Listener::~Listener() {
		Close();
	}

	void Listener::Listen(uint16_t port) {
		Close();
		if ((m_socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
			Logger::Error(GetLastError("socket"));
			return;
		}
		Address address = Address::Localhost(port);
		if (bind(m_socket_id, address.GetSockAddress(), address.GetSockAddressSize()) == SOCKET_ERROR) {
			Logger::Error(GetLastError("bind"));
			return;
		}
		if (listen(m_socket_id, 10) == -1) {
			Logger::Error(GetLastError("listen"));
			return;
		}
		Logger::Info("Socket listening on port: " + std::to_string(port));
	}

	void Listener::AcceptConnections(std::vector<Socket>& connections) {
		sockaddr_storage address_storage;
		socklen_t address_size = sizeof(address_storage);
		sockaddr* address_storage_ptr = reinterpret_cast<sockaddr*>(&address_storage);
		int new_socket_id = static_cast<int>(accept(m_socket_id, address_storage_ptr, &address_size));

		//if (new_socket_id == INVALID_SOCKET)
		//	Logger::Error(GetLastError("accept"));
		//else
		//	connections.emplace_back(new_socket_id, nullptr);
	}
}

















//#include "zore/networking/Socket.hpp"
//#include "zore/networking/Platform.hpp"
//#include "zore/Debug.hpp"
//
//namespace zore::Net {
//
//	//========================================================================
//	//	An abstract generic socket class
//	//========================================================================
//
//	Socket::Socket(const Address& address, Protocol protocol) : m_socket_id(INVALID_SOCKET), m_results(nullptr) {
//		int status;
//		addrinfo hints;
//		memset(&hints, 0, sizeof(addrinfo));
//
//		hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
//		static constexpr int PROTOCOL_TO_SOCKET_TYPE[] = { SOCK_STREAM, SOCK_DGRAM };
//		hints.ai_socktype = PROTOCOL_TO_SOCKET_TYPE[static_cast<int>(protocol)];
//		hints.ai_flags = AI_PASSIVE; // For wildcard IP address
//		Address& address_ref = const_cast<Address&>(address);
//
//
//		sizeof(sockaddr);
//		sizeof(sockaddr_in);
//		sizeof(sockaddr_in6);
//
//		// Get address info on the provided address
//		if ((status = getaddrinfo(address_ref.GetNode(), address_ref.GetPort(), &hints, &m_results)) != 0) {
//			Logger::Error("getaddrinfo error when connecting to " + std::string(address_ref.GetNode()) + "\n" + std::string(gai_strerror(status)));
//			return;
//		}
//		// Initialize the socket file descriptor
//		if ((m_socket_id = socket(m_results->ai_family, m_results->ai_socktype, m_results->ai_protocol)) == INVALID_SOCKET) {
//			Logger::Error(GetLastError("socket"));
//			return;
//		}
//	}
//
//	Socket::~Socket() {
//		closesocket(m_socket_id);
//	}
//
//	bool Socket::IsValid() {
//		return m_socket_id != INVALID_SOCKET;
//	}
//
//	//========================================================================
//	//	An active socket, used for external connections
//	//========================================================================
//
//	static int s_host_port = 3490;
//
//	ActiveSocket::ActiveSocket(const std::string& address, int port, Protocol protocol) : Socket(address.c_str(), port, protocol) {
//		if (!m_results)
//			return;
//		// Connect to a remote port
//		if (connect(m_socket_id, m_results->ai_addr, static_cast<int>(m_results->ai_addrlen)) == -1) {
//			Logger::Error(GetLastError("connect"));
//			return;
//		}
//		Logger::Log("Connected");
//
//		freeaddrinfo(m_results);
//	}
//
//	std::string ActiveSocket::GetPeerName() {
//		sockaddr address;
//		int address_length = sizeof(address);
//
//		if (getpeername(m_socket_id, &address, &address_length) == -1) {
//			Logger::Error(GetLastError("getpeername"));
//			return "Failed to get peername";
//		}
//		return "TODO: Get peername from sockaddr";
//	}
//
//	bool ActiveSocket::SendPacket(const void* data, int* length) {
//		int total_sent = 0, sent = 0;
//		const char* char_data = reinterpret_cast<const char*>(data);
//
//		while (total_sent < *length) {
//			sent = send(m_socket_id, char_data + total_sent, *length - total_sent, 0);
//			if (sent == -1)
//				break;
//			total_sent += sent;
//		}
//
//		*length = total_sent;
//		return (sent == -1) ? false : true;
//	}
//
//	bool ActiveSocket::ReceivePacket(void* buffer, int* length) {
//		char* char_buffer = reinterpret_cast<char*>(buffer);
//		*length = recv(m_socket_id, char_buffer, *length, 0);
//		return (*length == -1) ? false : true;
//	}
//
//	//========================================================================
//	//	A passive socket, used for listening for incoming connections
//	//========================================================================
//
//	PassiveSocket::PassiveSocket(int port, Protocol protocol) : Socket("", port, protocol) {
//		if (!m_results)
//			return;
//
//		// Bind our socket to a port
//		if (bind(m_socket_id, m_results->ai_addr, static_cast<int>(m_results->ai_addrlen)) == -1) {
//			Logger::Error(GetLastError("bind"));
//			return;
//		}
//
//		// Listen for incoming connections on a port
//		if (listen(m_socket_id, 10) == -1) {
//			Logger::Error(GetLastError("listen"));
//			return;
//		}
//		Logger::Info("Socket listening on port: " + std::to_string(port));
//
//		freeaddrinfo(m_results); // free the linked-list
//	}
//
//	void PassiveSocket::AcceptConnections(std::vector<ActiveSocket>& connections) {
//		sockaddr_storage address_storage;
//		socklen_t address_size = sizeof(address_storage);
//		sockaddr* address_storage_ptr = reinterpret_cast<sockaddr*>(&address_storage);
//		int new_socket_id = static_cast<int>(accept(m_socket_id, address_storage_ptr, &address_size));
//
//		if (new_socket_id == -1)
//			Logger::Error(GetLastError("accept"));
//		else
//			connections.emplace_back(new_socket_id, nullptr);
//	}
//}