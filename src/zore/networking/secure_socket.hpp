#pragma once

#include "zore/networking/socket.hpp"
#include "zore/networking/packet_buffer.hpp"
#include <tlse.h>

namespace zore::net {

	//========================================================================
	//	Secure (TLS) Socket
	//========================================================================

	class SecureSocket : public Socket {   
	public:
        SecureSocket(Protocol protocol = Protocol::TCP, bool blocking = true);
		SecureSocket(const Address& address, Protocol protocol = Protocol::TCP, bool blocking = true);
		~SecureSocket() override;

		Status Connect(const Address& address) override;
		Status Send(const void* data, uint32_t size) override;
		Status Receive(void* data, uint32_t size, uint32_t& recieved) override;
		void Close() override;

	private:
		Status InitializeContext(const Address& address);
		Status PerformHandshake();
		Status SendEncrypted();
		Status ReceiveEncrypted();

	private:
		struct TLSContext* m_tls_context;
		PacketBuffer m_encrypted{ 16 * 1024 };
		bool m_established = false;
	};
}
