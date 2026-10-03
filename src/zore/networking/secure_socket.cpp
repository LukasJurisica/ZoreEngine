#include "zore/networking/secure_socket.hpp"
#include "zore/networking/networking_core.hpp"
#include "zore/networking/ca_bundle.hpp"
#include "zore/debug.hpp"
#include <sys/types.h>

namespace zore::net {






	//int ValidateCertificate(
	//	TLSContext* context,
	//	TLSCertificate** chain,
	//	int chain_length)
	//{
	//	Logger::Info(std::format(
	//		"TLS presented {} certificates",
	//		chain_length
	//	));

	//	if (!chain || chain_length <= 0) {
	//		Logger::Error("TLS server presented no certificates");
	//		return certificate_unknown;
	//	}

	//	for (int i = 0; i < chain_length; ++i) {
	//		const int result =
	//			tls_certificate_is_valid(chain[i]);

	//		// Certificate helpers return 0 on success.
	//		if (result != 0) {
	//			Logger::Error(std::format(
	//				"TLS certificate {} date validation failed: {}",
	//				i,
	//				result
	//			));

	//			return result;
	//		}
	//	}

	//	int result = tls_certificate_chain_is_valid(
	//		chain,
	//		chain_length
	//	);

	//	if (result != 0) {
	//		Logger::Error(std::format(
	//			"TLS certificate chain validation failed: {}",
	//			result
	//		));

	//		return result;
	//	}

	//	const char* server_name = tls_sni(context);

	//	if (!server_name) {
	//		Logger::Error(
	//			"TLS certificate validation has no server name"
	//		);

	//		return certificate_unknown;
	//	}

	//	result = tls_certificate_valid_subject(
	//		chain[0],
	//		server_name
	//	);

	//	if (result != 0) {
	//		Logger::Error(std::format(
	//			"TLS certificate does not match '{}': {}",
	//			server_name,
	//			result
	//		));

	//		return result;
	//	}

	//	result = tls_certificate_chain_is_valid_root(
	//		context,
	//		chain,
	//		chain_length
	//	);

	//	if (result != 0) {
	//		Logger::Error(std::format(
	//			"TLS certificate is not rooted in a trusted CA: {}",
	//			result
	//		));

	//		return result;
	//	}

	//	Logger::Info("TLS certificate validation succeeded");

	//	// The callback protocol uses 255 for acceptance.
	//	return no_error;
	//}






	SecureSocket::SecureSocket(Protocol protocol, bool blocking) : Socket(protocol, blocking), m_tls_context(nullptr) {}

	SecureSocket::SecureSocket(const Address& address, Protocol protocol, bool blocking) : Socket(protocol, blocking), m_tls_context(nullptr) {
		Connect(address);
	}

	SecureSocket::~SecureSocket() {
		Close();
	}

	Socket::Status SecureSocket::Connect(const Address& address) {
		Close();
		if (!address.IsValid() || address.GetHostname().empty()) {
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

	Socket::Status SecureSocket::Send(const void* data, uint32_t size) {
		if (!m_tls_context || !m_established)
			return Status::DISCONNECTED;
		if (!data || size == 0)
			return Status::ERROR;

		const auto* bytes = static_cast<const unsigned char*>(data);
		uint32_t written = 0;

		while (written < size) {
			const int result = tls_write(m_tls_context, bytes + written, size - written);
			if (result <= 0)
				return Status::ERROR;
			written += static_cast<uint32_t>(result);

			const Status status = SendEncrypted();
			if (status != Status::DONE)
				return status;
		}

		return Status::DONE;
	}

	Socket::Status SecureSocket::Receive(void* data, uint32_t size, uint32_t& received) {
		received = 0;

		if (!m_tls_context || !m_established)
			return Status::DISCONNECTED;
		if (!data || size == 0)
			return Status::ERROR;

		while (true) {
			const int plain_size = tls_read(m_tls_context, static_cast<unsigned char*>(data), size);
			if (plain_size < 0)
				return Status::ERROR;
			if (plain_size > 0) {
				received = static_cast<uint32_t>(plain_size);
				return Status::DONE;
			}

			const Status status = ReceiveEncrypted();
			if (status != Status::DONE)
				return status;
		}
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

	Socket::Status SecureSocket::InitializeContext(const Address& address) {

		if (m_protocol != Protocol::TCP) {
			Logger::Error("SecureSocket currently supports TLS/TCP only");
			return Status::ERROR;
		}

		static constexpr uint16_t TLS_VERSION_MAP[] = { TLS_V12, DTLS_V12 };
		m_tls_context = tls_create_context(0, TLS_VERSION_MAP[static_cast<int>(m_protocol)]);
		if (!m_tls_context) {
			Logger::Error("Failed to initialize TLS context");
			return Status::ERROR;
		}
		if (address.GetHostType() == Address::HostType::DNS) {
			if (tls_sni_set(m_tls_context, address.GetHostname().c_str()) < 0) {
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
		if (SendEncrypted() != Status::DONE)
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

			if (Status status = ReceiveEncrypted(); status != Status::DONE)
				return status;
		}
	}

	Socket::Status SecureSocket::SendEncrypted() {
		unsigned int size = 0;
		const unsigned char* data = tls_get_write_buffer(m_tls_context, &size);

		if (!data || size == 0)
			return Status::DONE;
		Status status = Socket::Send(data, size);
		tls_buffer_clear(m_tls_context);
		return status;
	}

	Socket::Status SecureSocket::ReceiveEncrypted() {
		uint32_t received;
		if (Status status = Socket::Receive(m_encrypted.data(), m_encrypted.size(), received); status != Status::DONE)
			return status;

		const int consumed = tls_consume_stream(m_tls_context, m_encrypted, received, tls_default_verify);
		if (consumed < 0) {
			Logger::Error("TLS input processing failed with code:", consumed);
			return Status::ERROR;
		}
		return SendEncrypted();
	}
}
