#pragma once

#include "zore/networking/address.hpp"
#include "zore/networking/networking_types.hpp"
#include <chrono>

namespace zore::net {

	//========================================================================
	//	Base Socket
	//========================================================================

	class Socket {
	public:
		friend class Manager;

	public:
		enum class Status : uint8_t { DONE, WOULD_BLOCK, TIMED_OUT, ERROR, DISCONNECTED };

		struct Result {
			Status status;
			uint32_t bytes_transferred = 0;
		};

	public:
		Socket(Protocol protocol, bool blocking);
		Socket(socket_t socket_id, Protocol protocol, bool blocking);
		Socket(const Socket&) = delete;
		Socket(Socket&& other) noexcept;
		Socket& operator=(const Socket&) = delete;
		Socket& operator=(Socket&& other) noexcept;
		virtual ~Socket();

	public:
		Status SetBlocking(bool blocking);
		Status SetTimeout(std::chrono::milliseconds timeout);
		virtual Status Connect(const Address& address);
		virtual Result Send(const void* data, int size);
		virtual Result Receive(void* buffer, int max_size);
		virtual void Close();

		Address GetSelfAddress() const;
		Address GetPeerAddress() const;

	protected:
		void Move(Socket& other);
		Status Open();
		Status ApplyBlocking();
		Status ApplyTimeout();
		static int GetLastErrorCode();
		static std::string GetLastError(const std::string& function, int error_code = GetLastErrorCode());
		static Status GetErrorType(int error_code = GetLastErrorCode());

	protected:
		socket_t m_socket_id;
		uint32_t m_timeout;
		Protocol m_protocol;
		bool m_blocking;
	};
}
