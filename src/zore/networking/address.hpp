#pragma once

#include "zore/networking/networking_types.hpp"
#include <string>
#include <ostream>

namespace zore::net {

	class Address {
	public:
		friend class Socket;
		enum class Family { INVALID = -1, IPv4, IPv6 };

	public:
		Address(uint32_t ip, uint16_t port);
		Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port);
		//Address(const uint8_t ip[16], uint16_t port);
		Address(const Address& other);
		Address& operator=(const Address& other);
		operator std::string() const;
		~Address();

		static Address Localhost(uint16_t port);
		static Address Local();
		static Address Public();
		static Address Parse(const std::string& ip, uint16_t port);
		static Address Resolve(const std::string& hostname, uint16_t port, Protocol protocol);

		void SetPort(uint16_t port);
		bool IsValid() const;
		int GetFamily() const;
		const sockaddr* GetSockAddress() const;
		sockaddr_length_t GetSockAddressSize() const;
		friend std::ostream& operator<<(std::ostream& os, const Address& address);

	private:
		Address() = default;
		Address(const sockaddr_storage* storage);

		void Init(Family family = Family::INVALID, const sockaddr_storage* storage = nullptr);
		void InitIPv4(uint32_t ip, uint16_t port);
		void InitIPv6(const uint8_t ip[16], uint16_t port);
		bool ParseIPv4(const std::string& ip, uint16_t port);
		bool ParseIPv6(const std::string& ip, uint16_t port);
		static inline Family ConvertFamily(int family);

	private:
		sockaddr_storage* m_storage = nullptr;
		Family m_family = Family::INVALID;
	};
}