#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/span.hpp"
#include <vector>

namespace zore::net {

	class Packet {
	public:
		friend class Socket;

	public:
		Packet();
		Packet(const VoidSpan& span);
		Packet(const void* payload, uint32_t length, uint8_t flags = 0);
		~Packet() = default;

		uint8_t* Data();
        uint16_t* Header();
        uint8_t* Payload();
		uint32_t Size() const;
		uint32_t HeaderSize() const;
		uint32_t PayloadSize() const;
		uint32_t Remaining() const;

		void Clear();
		void Reserve(uint32_t size);
        void Resize(uint32_t size);
		void Append(const VoidSpan& span);
		void Seek(uint32_t offset);
        uint16_t* ParseHeader();

		//------------------------------------------------------------------------
		//	Read from the data stream
		//------------------------------------------------------------------------

		Packet& operator>>(bool& data);
		Packet& operator>>(int8_t& data);
		Packet& operator>>(uint8_t& data);
		Packet& operator>>(int16_t& data);
		Packet& operator>>(uint16_t& data);
		Packet& operator>>(int32_t& data);
		Packet& operator>>(uint32_t& data);
		Packet& operator>>(int64_t& data);
		Packet& operator>>(uint64_t& data);
		Packet& operator>>(float& data);
		Packet& operator>>(double& data);

		//------------------------------------------------------------------------
		//	Write to the data stream
		//------------------------------------------------------------------------

		Packet& operator<<(bool data);
		Packet& operator<<(int8_t data);
		Packet& operator<<(uint8_t data);
		Packet& operator<<(int16_t data);
		Packet& operator<<(uint16_t data);
		Packet& operator<<(int32_t data);
		Packet& operator<<(uint32_t data);
		Packet& operator<<(int64_t data);
		Packet& operator<<(uint64_t data);
		Packet& operator<<(float data);
		Packet& operator<<(double data);

    private:
        void SetHeader(uint16_t packet_id, uint16_t sequence_id, uint8_t version, uint8_t flags);

	private:
		uint32_t m_position;
		std::vector <uint8_t> m_data;
	};
}