#include "zore/networking/socket.hpp"
#include "zore/networking/networking_core.hpp"
#include "zore/debug.hpp"

namespace zore::net {

	//========================================================================
	//	Base Socket
	//========================================================================

	Socket::Socket(Protocol protocol, bool blocking) : Socket(INVALID_SOCKET, protocol, blocking) {}

	Socket::Socket(socket_t socket_id, Protocol protocol, bool blocking) : m_socket_id(socket_id), m_protocol(protocol), m_blocking(blocking), m_timeout(0) {}
	
	Socket::Socket(Socket&& other) noexcept {
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

	Socket::Status Socket::SetBlocking(bool blocking) {
		m_blocking = blocking;
		return ApplyBlocking();
	}

	Socket::Status Socket::SetTimeout(std::chrono::milliseconds timeout) {
		m_timeout = timeout.count();
		return ApplyTimeout();
	}

	Socket::Status Socket::Connect(const Address& address) {
		Close();
		if (address.IsValid() == false) {
			Logger::Error("Socket connection error: Invalid address");
			return Status::ERROR;
		}

		if (Status status = Open(); status != Status::DONE)
			return status;

		if (connect(m_socket_id, address.GetSockAddress(), address.GetSockAddressSize()) == SOCKET_ERROR) {
			Logger::Error(GetLastError("connect"));
			return Status::ERROR;
		}
		return Status::DONE;
	}

	Socket::Result Socket::Send(const void* data, int size) {
		if (m_socket_id == INVALID_SOCKET)
			return { Status::DISCONNECTED };
		if (!data || size == 0)
			return { Status::ERROR };

		uint32_t sent = 0;
		while (sent < size) {
			int result = send(m_socket_id, static_cast<const char*>(data) + sent, size - sent, 0);
			if (result == 0)
				return { Status::DISCONNECTED, sent };
			if (result < 0) {
				int error_code = GetLastErrorCode();
				Status error_type = GetErrorType(error_code);
				Logger::Error(GetLastError("send", error_code));
				return { error_type, sent };
			}
			sent += result;
		}
		return { Status::DONE, sent };
	}

	Socket::Result Socket::Receive(void* buffer, int max_size) {
		if (m_socket_id == INVALID_SOCKET)
			return { Status::DISCONNECTED };
		if (!buffer || max_size == 0)
			return { Status::ERROR };

		int result = recv(m_socket_id, static_cast<char*>(buffer), max_size, 0);
		if (result == 0)
			return { Status::DISCONNECTED };
		else if (result < 0) {
			int error_code = GetLastErrorCode();
			Status error_type = GetErrorType(error_code);
			Logger::Error(GetLastError("recv", error_code));
			return { error_type };
		}
		return { Status::DONE, static_cast<uint32_t>(result) };
	}

	void Socket::Close() {
		if (m_socket_id != INVALID_SOCKET) {
#if defined(PLATFORM_WINDOWS)
			closesocket(m_socket_id);
#else
			close(m_socket_id);
#endif
			m_socket_id = INVALID_SOCKET;
		}
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
	void Socket::Move(Socket& other) {
		m_socket_id = other.m_socket_id;
		m_timeout = other.m_timeout;
		m_blocking = other.m_blocking;
		m_protocol = other.m_protocol;
		other.m_socket_id = INVALID_SOCKET;
		other.m_timeout = 0;
	}

	Socket::Status Socket::Open() {
		Close();

		static constexpr int SOCKET_TYPE_MAP[] = { SOCK_STREAM, SOCK_DGRAM };
		static constexpr int SOCKET_PROTOCOL_MAP[] = { IPPROTO_TCP, IPPROTO_UDP };
		int socket_type = SOCKET_TYPE_MAP[static_cast<int>(m_protocol)];
		int socket_protocol = SOCKET_PROTOCOL_MAP[static_cast<int>(m_protocol)];

		if ((m_socket_id = socket(AF_INET, socket_type, socket_protocol)) == INVALID_SOCKET) {
			Logger::Error(GetLastError("socket"));
			return Status::ERROR;
		}
		if (Status status = ApplyTimeout(); status != Status::DONE)
			return status;
		if (Status status = ApplyBlocking(); status != Status::DONE)
			return status;
		return Status::DONE;
	}

	Socket::Status Socket::ApplyBlocking() {
		if (m_socket_id == INVALID_SOCKET)
			return Status::DONE;

#if defined(PLATFORM_WINDOWS)
		u_long mode = (m_blocking) ? 0 : 1;
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

	Socket::Status Socket::ApplyTimeout() {
		if (m_socket_id == INVALID_SOCKET)
			return Status::DONE;

#if defined(PLATFORM_WINDOWS)
		const DWORD timeout = static_cast<DWORD>(m_timeout);
		const char* timeout_ptr = reinterpret_cast<const char*>(&timeout);
#else
		timeval timeout;
		timeout.tv_sec = m_timeout / 1000;
		timeout.tv_usec = (m_timeout % 1000) * 1000;
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

	int Socket::GetLastErrorCode() {
#if defined(PLATFORM_WINDOWS)
		return WSAGetLastError();
#else
		return errno;
#endif
	}

	std::string Socket::GetLastError(const std::string& function, int error_code) {
#if defined(PLATFORM_WINDOWS)
		std::string error_message = zore::WindowsException::GetErrorString(error_code);
#elif defined(PLATFORM_LINUX)
		std::string error_message(' ', strerrorlen_s(error_code));
		strerror_s(error_message.data(), error_message.length(), error_code);
#endif
		return std::format("Socket error {} executing \"{}\": {}", error_code, function, error_message);
	}

	Socket::Status Socket::GetErrorType(int error_code) {
		switch (error_code) {
			case SOCK_CONNECTION_TIMED_OUT:
				return Status::TIMED_OUT;
			case SOCK_ALREADY:
			case SOCK_WOULD_BLOCK:
				return Status::WOULD_BLOCK;
			case SOCK_CONNECTION_RESET:
			case SOCK_CONNECTION_ABORTED:
			case SOCK_NET_RESET:
			case SOCK_NOT_CONNECTED:
				return Status::DISCONNECTED;
			case SOCK_CONNECTION_REFUSED:
			case SOCK_HOST_UNREACHABLE:
			case SOCK_NETWORK_UNREACHABLE:
			default:
				return Status::ERROR;
		}
	}
}