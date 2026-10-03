#pragma once

#include "zore/networking/address.hpp"
#include "zore/networking/packet.hpp"
#include "zore/networking/networking_types.hpp"
#include <vector>
#include <chrono>

namespace zore::net {

	//========================================================================
	//	Base Socket
	//========================================================================

	class AbstractSocket {
	public:
		void SetBlocking(bool blocking);
		virtual void Close();

	protected:
		AbstractSocket(socket_t socket_id, bool blocking = true);
		AbstractSocket(const AbstractSocket&) = delete;
		AbstractSocket(AbstractSocket&& other) noexcept;
		AbstractSocket& operator=(const AbstractSocket&) = delete;
		AbstractSocket& operator=(AbstractSocket&& other) noexcept;
		virtual ~AbstractSocket();

		void Move(AbstractSocket& other);

	protected:
		socket_t m_socket_id;
		bool m_blocking;
	};

	//========================================================================
	//	Connection Socket
	//========================================================================

	class Socket : public AbstractSocket {
    public:
        enum class Status : uint8_t {
            DONE,
            WOULD_BLOCK,
			TIMED_OUT,
            DISCONNECTED,
            ERROR
        };

	public:
        Socket(Protocol protocol, bool blocking = false);
		Socket(socket_t socket_id, Protocol protocol, bool blocking = false);
		Socket(const Address& address, Protocol protocol, bool blocking = false);
		Socket(const Socket&) = delete;
		Socket(Socket&& other) noexcept;
		Socket& operator=(const Socket&) = delete;
		Socket& operator=(Socket&& other) noexcept;
		virtual ~Socket();

		Address GetSelfAddress() const;
		Address GetPeerAddress() const;

		Status SetTimeout(std::chrono::milliseconds timeout);
		virtual Status Disconnect();
		virtual Status Connect(const Address& address);
		virtual Status Send(Packet& packet);
		virtual Status Send(const void* data, uint32_t size);
		virtual Status Receive(Packet& packet);
		virtual Status Receive(void* data, uint32_t size, uint32_t& recieved);
		bool IsReady() const;

	protected:
		void Move(Socket& other);

	private:
		Status ApplyTimeout();
		static Status GetErrorType(int error_code);

	protected:
		std::chrono::milliseconds m_timeout;
		uint32_t m_sequence_id = 0;
		Protocol m_protocol;
	};

	//========================================================================
	//	Listener Socket
	//========================================================================

	class Listener : public AbstractSocket {
	public:
		Listener(bool blocking = false);
		~Listener();

		void Listen(uint16_t port);
		void AcceptConnections(std::vector<Socket>& connections, Protocol protocol);
	};
}
