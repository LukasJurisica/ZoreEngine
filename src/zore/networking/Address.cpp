#include "zore/networking/address.hpp"
#include "zore/networking/core.hpp"
#include "zore/networking/socket.hpp"
#include "zore/networking/http/client.hpp"
#include "zore/debug.hpp"

#define sockaddr_ipv4(x) reinterpret_cast<sockaddr_in*>(x)
#define sockaddr_ipv6(x) reinterpret_cast<sockaddr_in6*>(x)
#define sockaddr_any(x) reinterpret_cast<sockaddr_storage*>(x)

namespace zore::net {

	Address::Address(const sockaddr_storage* storage) {
		Init(Family::INVALID, storage);
	}

	Address::Address(uint32_t ip, uint16_t port) {
		InitIPv4(ip, port);
	}

	Address::Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port) {
		uint32_t ip = (static_cast<uint32_t>(a) << 24) |
			(static_cast<uint32_t>(b) << 16) |
			(static_cast<uint32_t>(c) << 8) |
			(static_cast<uint32_t>(d) << 0);
		InitIPv4(ip, port);
	}

	//Address::Address(const uint8_t ip[16], uint16_t port) {
	//	InitIPv6(ip, port);
	//}

	Address::Address(const Address& other) {
		if (other.m_storage)
			Init(other.m_family, other.m_storage);
	}

	Address& Address::operator=(const Address& other) {
		if (this != &other)
			Init(other.m_family, other.m_storage);
		return *this;
	}

	Address::operator std::string() const {
		if (m_storage == nullptr)
			return "Invalid Address";
		char buffer[INET6_ADDRSTRLEN];
		if (m_family == Family::IPv4) {
			sockaddr_in* ipv4 = sockaddr_ipv4(m_storage);
			if (inet_ntop(AF_INET, &ipv4->sin_addr, buffer, sizeof(buffer)) != nullptr)
				return std::format("{}:{}", buffer, ntohs(ipv4->sin_port));
		}
		else if (m_family == Family::IPv6) {
			sockaddr_in6* ipv6 = sockaddr_ipv6(m_storage);
			if (inet_ntop(AF_INET6, &ipv6->sin6_addr, buffer, sizeof(buffer)) != nullptr)
				return std::format("[{}]:{}", buffer, ntohs(ipv6->sin6_port));
		}
		return "Invalid Address";
	}

	Address::~Address() {
		delete m_storage;
		m_storage = nullptr;
	}

	Address Address::Localhost(uint16_t port) {
		return Address(127, 0, 0, 1, port);
	}

	Address Address::Local() {
		Socket sock(Protocol::UDP);
		if (sock.Connect({8, 8, 8, 8, 53}) != Socket::Status::DONE)
			return Address(nullptr);
		return sock.GetSelfAddress();
	}

	Address Address::Public() {
		using namespace zore::net::http;
		Client client("www.sfml-dev.org", 80);
		Request request(Request::Method::GET, "/ip-provider.php");
		Response response = client.Make(request);
		return Parse(response.GetBody(), 0);
	}

	Address Address::Parse(const std::string& ip, uint16_t port) {
		if (ip.length() == 0)
			return Localhost(port);

		Address address;
		if (address.ParseIPv4(ip, port) || address.ParseIPv6(ip, port))
			return address;
		Logger::Error(std::format("Failed to parse address: ({}:{})", ip, port));
		return Address();
	}

	Address Address::Resolve(const std::string& hostname, uint16_t port, Protocol protocol) {
		int status;
		addrinfo hints;
		memset(&hints, 0, sizeof(addrinfo));
		addrinfo* results = nullptr;

		static constexpr int PROTOCOL_TO_SOCKET_TYPE[] = { SOCK_STREAM, SOCK_DGRAM };
		hints.ai_socktype = PROTOCOL_TO_SOCKET_TYPE[static_cast<int>(protocol)];
		hints.ai_family = AF_UNSPEC;
		hints.ai_flags = AI_PASSIVE;

		Address address;
		if ((status = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &results)) == 0)
			address.Init(ConvertFamily(results->ai_family), sockaddr_any(results->ai_addr));
		else
			Logger::Error(std::format("getaddrinfo error when connecting to: ({}:{})\n{}", hostname, port, gai_strerror(status)));

		if (results)
			freeaddrinfo(results);
		return address;
	}

	void Address::SetPort(uint16_t port) {
		if (m_storage == nullptr)
			Logger::Error("Could not set port on address: Invalid Address Family.");
		else if (m_family == Family::IPv4)
			sockaddr_ipv4(m_storage)->sin_port = htons(port);
		else if (m_family == Family::IPv6)
			sockaddr_ipv6(m_storage)->sin6_port = htons(port);
		else
			Logger::Error("Could not set port on address: Invalid Address Family.");
	}

	bool Address::IsValid() const {
		return m_family != Family::INVALID && m_storage != nullptr;
	}

	int Address::GetFamily() const {
		static constexpr int FAMILY_TO_AI_FAMILY[] = { AF_INET, AF_INET6 };
		return FAMILY_TO_AI_FAMILY[static_cast<int>(m_family)];
	}

	const sockaddr* Address::GetSockAddress() const {
		return reinterpret_cast<sockaddr*>(m_storage);
	}

	sockaddr_length_t Address::GetSockAddressSize() const {
		switch (m_family) {
		case Family::IPv4: return sizeof(sockaddr_in);
		case Family::IPv6: return sizeof(sockaddr_in6);
		default: return 0;
		}
	}

	std::ostream& operator<<(std::ostream& os, const Address& address) {
		return os << std::string(address);
	}

	void Address::Init(Family family, const sockaddr_storage* storage) {
		m_family = family;
		if (m_storage == nullptr)
			m_storage = new sockaddr_storage();
		if (storage)
			*m_storage = *storage;
		else
			std::memset(m_storage, 0, sizeof(sockaddr_storage));
	}

	void Address::InitIPv4(uint32_t ip, uint16_t port) {
		Init(Family::IPv4);
		sockaddr_in* ipv4 = sockaddr_ipv4(m_storage);
		ipv4->sin_family = AF_INET;
		ipv4->sin_port = htons(port);
		ipv4->sin_addr.s_addr = htonl(ip);
	}

	void Address::InitIPv6(const uint8_t ip[16], uint16_t port) {
		Init(Family::IPv6);
		sockaddr_in6* ipv6 = sockaddr_ipv6(m_storage);
		ipv6->sin6_family = AF_INET6;
		ipv6->sin6_port = htons(port);
		std::memcpy(&ipv6->sin6_addr, ip, 16);
	}

	bool Address::ParseIPv4(const std::string& ip, uint16_t port) {
		Init(Family::IPv4);
		sockaddr_in* ipv4 = sockaddr_ipv4(m_storage);
		ipv4->sin_family = AF_INET;
		ipv4->sin_port = htons(port);
		return inet_pton(AF_INET, ip.c_str(), &ipv4->sin_addr) == 1;
	}

	bool Address::ParseIPv6(const std::string& ip, uint16_t port) {
		Init(Family::IPv6);
		sockaddr_in6* ipv6 = sockaddr_ipv6(m_storage);
		ipv6->sin6_family = AF_INET6;
		ipv6->sin6_port = htons(port);
		return inet_pton(AF_INET6, ip.c_str(), &ipv6->sin6_addr) == 1;
	}

	Address::Family Address::ConvertFamily(int family) {
		switch (family) {
		case AF_INET: return Family::IPv4;
		case AF_INET6: return Family::IPv6;
		default: return Family::INVALID;
		}
	}
}