#include "zore/networking/secure_socket.hpp"
#include "zore/networking/networking_core.hpp"
#include "zore/networking/ca_bundle.hpp"
#include "zore/debug.hpp"
#include <tlse.h>

namespace zore::net {

	SecureSocket::SecureSocket(Protocol protocol, bool blocking) : Socket(protocol, blocking), m_tls_context(nullptr), m_established(false) {
		ENSURE(protocol == Protocol::TCP, "SecureSocket currently only supports the TCP protocol");
	}

	SecureSocket::SecureSocket(const Address& address, Protocol protocol, bool blocking) : Socket(protocol, blocking), m_tls_context(nullptr), m_established(false) {
		ENSURE(protocol == Protocol::TCP, "SecureSocket currently only supports the TCP protocol");
		Connect(address);
	}

	SecureSocket::SecureSocket(SecureSocket&& other) noexcept : Socket(std::move(other)) {
		Move(other);
	}

	SecureSocket& SecureSocket::operator=(SecureSocket&& other) noexcept {
		if (this != &other) {
			Close();
			Move(other);
			Socket::Move(other);
		}
		return *this;
	}

	SecureSocket::~SecureSocket() {
		Close();
	}

	Socket::Status SecureSocket::Connect(const Address& address) {
		Close();
		if (!address.IsValid() || address.GetHostType() != Address::HostType::DNS || address.GetHostname().empty()) {
			Logger::Error("Socket connection error: Invalid address");
			return Status::ERROR;
		}
		if (Status status = Socket::Connect(address); status != Status::DONE)
			return status;
		if (Status status = InitializeContext(address); status != Status::DONE) {
			Close();
			return status;
		}
		return Status::DONE;
	}

	Socket::Result SecureSocket::Send(const void* data, int size) {
		if (!m_tls_context || !m_established)
			return { Status::DISCONNECTED };
		if (!data || size <= 0)
			return { Status::ERROR };

		const Result pending = SendEncrypted();
		if (pending.status != Status::DONE)
			return { pending.status, 0 };

		const unsigned char* bytes = static_cast<const unsigned char*>(data);
		uint32_t sent = 0;

		while (sent < size) {
			const int written = tls_write(m_tls_context, bytes + sent, size - sent);
			if (written <= 0)
				return { Status::ERROR, sent };

			sent += static_cast<uint32_t>(written);
			const Result flushed = SendEncrypted();
			if (flushed.status != Status::DONE)
				return { flushed.status, sent };
		}
		return { Status::DONE, sent };
	}

	Socket::Result SecureSocket::Receive(void* buffer, int max_size) {
		if (!m_tls_context || !m_established)
			return { Status::DISCONNECTED };
		if (!buffer || max_size == 0)
			return { Status::ERROR };

		while (true) {
			const int plain_size = tls_read(m_tls_context, static_cast<unsigned char*>(buffer), max_size);
			if (plain_size < 0) {
				Logger::Error("SecureSocket receive error: TLS read failed with error code: {}", plain_size);
				return { Status::ERROR };
			}
			if (plain_size > 0)
				return { Status::DONE, static_cast<uint32_t>(plain_size) };

			const Result result = ReceiveEncrypted();
			if (result.status != Status::DONE)
				return { result.status };
		}
	}

	Socket::Status SecureSocket::Flush() {
		if (!m_tls_context || !m_established)
			return Status::DISCONNECTED;
		return SendEncrypted().status;
	}

	void SecureSocket::Close() {
		if (m_tls_context) {
			if (m_established && m_socket_id != INVALID_SOCKET) {
				tls_close_notify(m_tls_context);
				SendEncrypted();
			}
			tls_destroy_context(m_tls_context);
			m_tls_context = nullptr;
		}
		m_established = false;
		Socket::Close();
	}

	void SecureSocket::Move(SecureSocket& other) {
		m_tls_context = other.m_tls_context;
		m_established = other.m_established;
		other.m_tls_context = nullptr;
		other.m_established = false;
	}

	Socket::Status SecureSocket::InitializeContext(const Address& address) {
		static constexpr uint16_t TLS_VERSION_MAP[] = { TLS_V12, DTLS_V12 };
		m_tls_context = tls_create_context(0, TLS_VERSION_MAP[static_cast<int>(m_protocol)]);
		if (!m_tls_context) {
			Logger::Error("Failed to initialize TLS context");
			return Status::ERROR;
		}
		if (address.GetHostType() == Address::HostType::DNS) {
			if (tls_sni_set(m_tls_context, address.GetHostname().c_str()) != 1) {
				Logger::Error("Failed to set SNI");
				return Status::ERROR;
			}
		}

		const int certificate_count = tls_load_root_certificates(m_tls_context, CA_BUNDLE, CA_BUNDLE_SIZE);
		if (certificate_count <= 0) {
			Logger::Error("Failed to load trusted CA certificates");
			return Status::ERROR;
		}
		if (tls_client_connect(m_tls_context) < 0) {
			Logger::Error("Failed to create TLS ClientHello");
			return Status::ERROR;
		}
		if (SendEncrypted().status != Status::DONE)
			return Status::ERROR;
		if (Status status = PerformHandshake(); status != Status::DONE)
			return status;
		return Status::DONE;
	}

	Socket::Status SecureSocket::PerformHandshake() {
		while (true) {
			const int established = tls_established(m_tls_context);
			if (established < 0) {
				Logger::Error("TLS handshake entered a critical error state");
				return Status::ERROR;
			}
			if (established == 1) {
				m_established = true;
				return Status::DONE;
			}

			if (Result result = ReceiveEncrypted(); result.status != Status::DONE)
				return result.status;
		}
	}

	Socket::Result SecureSocket::SendEncrypted() {
		unsigned int size = 0;
		const unsigned char* data = tls_get_write_buffer(m_tls_context, &size);

		if (m_encrypted_send_offset > size || (size != 0 && !data))
			return { Status::ERROR };

		uint32_t sent = 0;
		while (m_encrypted_send_offset < size) {
			uint32_t remaining = size - m_encrypted_send_offset;

			int to_send = static_cast<int>(std::min(remaining, static_cast<uint32_t>(int32_max)));
			const Result result = Socket::Send(data + m_encrypted_send_offset, to_send);

			m_encrypted_send_offset += result.bytes_transferred;
			sent += result.bytes_transferred;
			if (result.status != Status::DONE)
				return { result.status, sent };
		}

		tls_buffer_clear(m_tls_context);
		m_encrypted_send_offset = 0;
		return { Status::DONE, sent };
	}

	Socket::Result SecureSocket::ReceiveEncrypted() {
		if (!m_tls_context)
			return { Status::DISCONNECTED };

		const Result pending = SendEncrypted();
		if (pending.status != Status::DONE)
			return { pending.status, 0 };

		const Result received = Socket::Receive(m_encrypted.data(), m_encrypted.size());
		if (received.status != Status::DONE)
			return received;

		const int consumed = tls_consume_stream(m_tls_context, m_encrypted, static_cast<int>(received.bytes_transferred), tls_default_verify);
		if (consumed < 0) {
			Logger::Error("TLS input processing failed with code:", consumed);
			return { Status::ERROR, received.bytes_transferred };
		}

		const Result flushed = SendEncrypted();
		return { flushed.status, received.bytes_transferred };
	}
}