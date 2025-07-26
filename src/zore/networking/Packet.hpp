#pragma once

#include "zore/utils/datatypes.hpp"
#include "zore/utils/Span.hpp"
#include <vector>

namespace zore::net {

	class Packet {
	public:
		friend class Socket;

	public:
		Packet();
		Packet(const uint8_t* payload, uint16_t length, uint8_t flags = 0);
		~Packet() = default;

		uint8_t* Data();
        uint16_t* Header();
        uint8_t* Payload();
		size_t Size() const;
		size_t HeaderSize() const;
		size_t PayloadSize() const;
		size_t Remaining() const;

		void Clear();
		void Reserve(size_t size);
        void Resize(size_t size);
		void Append(const VoidSpan& span);
		void Seek(uint16_t offset);
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
		size_t m_position;
		std::vector <uint8_t> m_data;
	};
}