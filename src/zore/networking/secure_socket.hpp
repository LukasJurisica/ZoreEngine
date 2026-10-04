#pragma once

#include "zore/networking/socket.hpp"
#include "zore/networking/packet_buffer.hpp"

struct TLSContext;

namespace zore::net {

	//========================================================================
	//	Secure Socket
	//========================================================================

	class SecureSocket : public Socket {
	public:
		SecureSocket(Protocol protocol = Protocol::TCP, bool blocking = true);
		SecureSocket(const Address& address, Protocol protocol = Protocol::TCP, bool blocking = true);
		SecureSocket(const SecureSocket&) = delete;
		SecureSocket(SecureSocket&& other) noexcept;
		SecureSocket& operator=(const SecureSocket&) = delete;
		SecureSocket& operator=(SecureSocket&& other) noexcept;
		~SecureSocket() override;

		Status Connect(const Address& address) override;
		Result Send(const void* data, int size) override;
		Result Receive(void* buffer, int max_size) override;
		Status Flush();
		void Close() override;

	private:
		void Move(SecureSocket& other);
		Status InitializeContext(const Address& address);
		Status PerformHandshake();
		Result SendEncrypted();
		Result ReceiveEncrypted();

	private:
		PacketBuffer m_encrypted{ 16 * 1024 };
		TLSContext* m_tls_context = nullptr;
		bool m_established = false;
		uint32_t m_encrypted_send_offset = 0;
	};
}
