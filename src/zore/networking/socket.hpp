#pragma once

#include "zore/networking/address.hpp"
#include "zore/networking/packet.hpp"
#include "zore/networking/networking_types.hpp"
#include <vector>

namespace zore::net {

	//========================================================================
	//	Base Socket
	//========================================================================

	class AbstractSocket {
	public:
		void SetBlocking(bool blocking);
		void Close();

	protected:
		AbstractSocket(socket_t socket_id, bool blocking = true);
		virtual ~AbstractSocket();

	protected:
		socket_t m_socket_id;
		bool m_blocking;
	};

	//========================================================================
	//	Connection Socket
	//========================================================================

	class Socket : public AbstractSocket {
    public:
        enum class Status {
            DONE,
            READY,
            DISCONNECTED,
            ERROR
        };
    
	public:
        Socket(Protocol protocol, bool blocking = false);
		Socket(const Address& address, Protocol protocol, bool blocking = false);
		Socket(socket_t socket_id, Protocol protocol, bool blocking = false);
		~Socket();

		Address GetSelfAddress() const;
		Address GetPeerAddress() const;

		Status Connect(const Address& address);
		Status Send(Packet& packet);
		Status Send(const void* data, uint32_t size);
		Status Receive(Packet& packet);
		Status Receive(void* data, uint32_t size, uint32_t& recieved);
        Status GetStatus() const;

	private:
		int m_socket_type;
		int m_socket_protocol;
        uint32_t m_sequence_id = 0;
	};

	//========================================================================
	//	Listener Socket
	//========================================================================

	class Listener : public AbstractSocket {
	public:
		Listener(bool blocking = false);
		~Listener();

		void Listen(uint16_t port);
		void AcceptConnections(std::vector<Socket>& connections);
	};
}