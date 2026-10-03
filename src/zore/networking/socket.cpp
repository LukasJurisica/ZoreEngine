#include "zore/networking/socket.hpp"
#include "zore/networking/networking_core.hpp"
#include "zore/debug.hpp"
#include <sys/types.h>

namespace zore::net {

	//========================================================================
	//	Base Socket
	//========================================================================

	AbstractSocket::AbstractSocket(socket_t socket_id, bool blocking) : m_socket_id(socket_id) {
		SetBlocking(blocking);
	}

	AbstractSocket::AbstractSocket(AbstractSocket&& other) noexcept : m_socket_id(other.m_socket_id), m_blocking(other.m_blocking) {
		other.m_socket_id = INVALID_SOCKET;
	}

	AbstractSocket& AbstractSocket::operator=(AbstractSocket&& other) noexcept {
		if (this != &other)
			Move(other);
		return *this;
	}

	AbstractSocket::~AbstractSocket() {
		Close();
	}

	void AbstractSocket::Move(AbstractSocket& other) {
		Close();
		m_socket_id = other.m_socket_id;
		m_blocking = other.m_blocking;
		other.m_socket_id = INVALID_SOCKET;
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

	Socket::Socket(Protocol protocol, bool blocking) : AbstractSocket(INVALID_SOCKET, blocking), m_protocol(protocol) {
		SetTimeout(std::chrono::milliseconds::zero());
	}

	Socket::Socket(socket_t socket_id, Protocol protocol, bool blocking) : AbstractSocket(socket_id, blocking), m_protocol(protocol) {
		SetTimeout(std::chrono::milliseconds::zero());
	}

	Socket::Socket(const Address& address, Protocol protocol, bool blocking) : AbstractSocket(INVALID_SOCKET, blocking), m_protocol(protocol) {
		SetTimeout(std::chrono::milliseconds::zero());
		Connect(address);
	}

	Socket::Socket(Socket&& other) noexcept : AbstractSocket(INVALID_SOCKET) {
		Move(other);
	}

	Socket& Socket::operator=(Socket&& other) noexcept {
		if (this != &other)
			Move(other);
		return *this;
	}

	Socket::~Socket() {
		Close();
	}

	Address Socket::GetSelfAddress() const {
		sockaddr_in address;
		int address_size = sizeof(address);
		if (getsockname(m_socket_id, reinterpret_cast<sockaddr*>(&address), &address_size) == SOCKET_ERROR) {
			Logger::Error(GetLastError("getsockname"));
			return Address(nullptr);
		}
		return Address(ntohl(address.sin_addr.s_addr), ntohs(address.sin_port));
	}

	Address Socket::GetPeerAddress() const {
		sockaddr_in address;
		int address_size = sizeof(address);
		if (getpeername(m_socket_id, reinterpret_cast<sockaddr*>(&address), &address_size) == SOCKET_ERROR) {
			Logger::Error(GetLastError("getpeername"));
			return Address(nullptr);
		}
		return Address(ntohl(address.sin_addr.s_addr), ntohs(address.sin_port));
	}

	Socket::Status Socket::SetTimeout(std::chrono::milliseconds timeout) {
		m_timeout = std::max(timeout, std::chrono::milliseconds::zero());
		return ApplyTimeout();
	}

	Socket::Status Socket::Disconnect() {
		Close();
		return Status::DISCONNECTED;
	}

	Socket::Status Socket::Connect(const Address& address) {
		Close();
		m_sequence_id = 0;
		if (address.IsValid() == false) {
			Logger::Error("Socket connection error: Invalid address");
			return Status::ERROR;
		}

		static constexpr int SOCKET_TYPE_MAP[] = { SOCK_STREAM, SOCK_DGRAM };
		static constexpr int SOCKET_PROTOCOL_MAP[] = { IPPROTO_TCP, IPPROTO_UDP };
		int socket_type = SOCKET_TYPE_MAP[static_cast<int>(m_protocol)];
		int socket_protocol = SOCKET_PROTOCOL_MAP[static_cast<int>(m_protocol)];

		if ((m_socket_id = socket(address.GetFamily(), socket_type, socket_protocol)) == INVALID_SOCKET) {
			Logger::Error(GetLastError("socket"));
			return Status::ERROR;
		}
		if (connect(m_socket_id, address.GetSockAddress(), address.GetSockAddressSize()) == SOCKET_ERROR) {
			Logger::Error(GetLastError("connect"));
			return Status::ERROR;
		}
		if (Status status = ApplyTimeout(); status != Status::DONE)
			return status;
		SetBlocking(m_blocking);
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
			if (result == 0)
				return Disconnect();
			if (result < 0) {
				int error_code = GetLastErrorCode();
				Status error_type = GetErrorType(error_code);
				Logger::Error(GetLastError("send", error_code));
				return error_type;
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
			return Disconnect();
		else if (result < 0) {
			int error_code = GetLastErrorCode();
			Status error_type = GetErrorType(error_code);
			Logger::Error(GetLastError("recv", error_code));
			return error_type;
		}
		recieved += result;
		return Status::DONE;
	}

	bool Socket::IsReady() const {
		return m_socket_id != INVALID_SOCKET;
	}

	void Socket::Move(Socket& other) {
		AbstractSocket::Move(other);
		m_protocol = other.m_protocol;
		m_sequence_id = other.m_sequence_id;
		m_timeout = other.m_timeout;
		other.m_timeout = std::chrono::milliseconds::zero();
	}

	Socket::Status Socket::ApplyTimeout() {
		if (m_socket_id == INVALID_SOCKET)
			return Status::DONE;

#if defined(PLATFORM_WINDOWS)
		const DWORD timeout = static_cast<DWORD>(std::max<int64_t>(0, m_timeout.count()));
		const char* timeout_ptr = reinterpret_cast<const char*>(&timeout);
#else
		timeval timeout;
		timeout.tv_sec = m_timeout.count() / 1000;
		timeout.tv_usec = (m_timeout.count() % 1000) * 1000;
		const timeval* timeout_ptr = &timeout;
#endif

		if (setsockopt(m_socket_id, SOL_SOCKET, SO_SNDTIMEO, timeout_ptr, sizeof(timeout)) == SOCKET_ERROR) {
			Logger::Error(GetLastError("setsockopt SO_SNDTIMEO"));
			return Status::ERROR;
		}
		if (setsockopt(m_socket_id, SOL_SOCKET, SO_RCVTIMEO, timeout_ptr, sizeof(timeout)) == SOCKET_ERROR) {
			Logger::Error(GetLastError("setsockopt SO_RCVTIMEO"));
			return Status::ERROR;
		}
		return Status::DONE;
	}

	Socket::Status Socket::GetErrorType(int error_code) {
#if defined(PLATFORM_WINDOWS)
		switch (error_code) {
		case WSAEWOULDBLOCK:
			return Status::WOULD_BLOCK;
		case WSAETIMEDOUT:
			return Status::TIMED_OUT;
		default:
			return Status::ERROR;
		}
#else
		switch (error_code) {
		case EAGAIN:
		case EWOULDBLOCK:
			return Status::WOULD_BLOCK;
		case ETIMEDOUT:
			return Status::TIMED_OUT;
		default:
			return Status::ERROR;
	}
#endif
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
		char enable = 1;
		if ((m_socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
			Logger::Error(GetLastError("socket"));
			return;
		}
		setsockopt(m_socket_id, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(char));
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

	void Listener::AcceptConnections(std::vector<Socket>& connections, Protocol protocol) {
		sockaddr_storage address_storage;
		socklen_t address_size = sizeof(address_storage);
		sockaddr* address_storage_ptr = reinterpret_cast<sockaddr*>(&address_storage);
		socket_t new_socket_id = accept(m_socket_id, address_storage_ptr, &address_size);

		if (new_socket_id == INVALID_SOCKET)
			Logger::Error(GetLastError("accept"));
		else
			connections.emplace_back(new_socket_id, protocol, false);
	}
}
