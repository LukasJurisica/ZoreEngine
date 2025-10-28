#include "zore/networking/packet.hpp"
#include "zore/networking/networking_utils.hpp"

namespace zore::net {

    Packet::Packet() {
       Resize(HeaderSize());
    }

	Packet::Packet(const VoidSpan& span) : Packet(span.Data(), static_cast<uint32_t>(span.Size())) {}

	Packet::Packet(const void* payload, uint32_t length, uint8_t flags) : m_position(0) {
		if (payload && (length > 0)) {
			Resize(HeaderSize() + length);
			std::memcpy(m_data.data() + HeaderSize(), payload, length);
		} else {
            Resize(HeaderSize());
        }
	}

	uint8_t* Packet::Data() {
		return m_data.data();
	}

    uint16_t* Packet::Header() {
        return reinterpret_cast<uint16_t*>(m_data.data());
    }

    uint8_t* Packet::Payload() {
        return m_data.data() + HeaderSize();
    }

	uint32_t Packet::Size() const {
		return static_cast<uint32_t>(m_data.size());
	}

	uint32_t Packet::HeaderSize() const {
		return sizeof(uint16_t) * 3 + sizeof(uint8_t) * 2;
	}

	uint32_t Packet::PayloadSize() const {
		return Size() - HeaderSize();
	}

	uint32_t Packet::Remaining() const {
		return Size() - m_position;
	}

	void Packet::Clear() {
		Resize(HeaderSize());
	}

	void Packet::Reserve(uint32_t size) {
		m_data.reserve(size);
	}

    void Packet::Resize(uint32_t size) {
        m_data.resize(size < HeaderSize() ? HeaderSize() : size);
    }

	void Packet::Append(const VoidSpan& span) {
		if (span.Data() && (span.Size() > 0))
			m_data.insert(m_data.end(), span.Data<uint8_t>(), span.Data<uint8_t>() + span.SizeBytes());
	}

	void Packet::Seek(uint32_t offset) {
		m_position = (offset < Size()) ? offset : Size();
	}

    uint16_t* Packet::ParseHeader() {
        if (Size() < HeaderSize())
            return nullptr;
        uint16_t* header = Header();
        header[0] = ntoz(header[0]); // packet id
        header[1] = ntoz(header[1]); // sequence id
        header[2] = ntoz(header[2]); // size
        return header;
    }
    
    void Packet::SetHeader(uint16_t packet_id, uint16_t sequence_id, uint8_t version, uint8_t flags) {
        uint16_t* header = Header();
        header[0] = zton(packet_id);
        header[1] = zton(sequence_id);
        header[2] = zton(Size());
        header[3] = static_cast<uint16_t>(version) << 8 | static_cast<uint16_t>(flags);
    }

	//------------------------------------------------------------------------
	//	Read from the data stream
	//------------------------------------------------------------------------

	Packet& Packet::operator>>(bool& data) {
		if (Remaining() >= sizeof(data)) {
			data = m_data[m_position] != 0;
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(int8_t& data) {
		if (Remaining() >= sizeof(data)) {
			data = static_cast<int8_t>(m_data[m_position]);
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(uint8_t& data) {
		if (Remaining() >= sizeof(data)) {
            data = m_data[m_position];
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(int16_t& data) {
		if (Remaining() >= sizeof(data)) {
            std::memcpy(&data, &m_data[m_position], sizeof(data));
            data = ntoz(data);
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(uint16_t& data) {
		if (Remaining() >= sizeof(data)) {
            std::memcpy(&data, &m_data[m_position], sizeof(data));
            data = ntoz(data);
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(int32_t& data) {
		if (Remaining() >= sizeof(data)) {
			std::memcpy(&data, &m_data[m_position], sizeof(data));
			data = ntoz(data);
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(uint32_t& data) {
		if (Remaining() >= sizeof(data)) {
			std::memcpy(&data, &m_data[m_position], sizeof(data));
			data = ntoz(data);
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(int64_t& data) {
		if (Remaining() >= sizeof(data)) {
			std::memcpy(&data, &m_data[m_position], sizeof(data));
			data = ntoz(data);
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(uint64_t& data) {
		if (Remaining() >= sizeof(data)) {
			std::memcpy(&data, &m_data[m_position], sizeof(data));
			data = ntoz(data);
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(float& data) {
		if (Remaining() >= sizeof(data)) {
			std::memcpy(&data, &m_data[m_position], sizeof(data));
			m_position += sizeof(data);
		}
		return *this;
	}

	Packet& Packet::operator>>(double& data) {
		if (Remaining() >= sizeof(data)) {
			std::memcpy(&data, &m_data[m_position], sizeof(data));
			m_position += sizeof(data);
		}
		return *this;
	}

	//------------------------------------------------------------------------
	//	Write to the data stream
	//------------------------------------------------------------------------

	Packet& Packet::operator<<(bool data) {
        Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(int8_t data) {
		Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(uint8_t data) {
		Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(int16_t data) {
		data = zton(data);
		Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(uint16_t data) {
		data = zton(data);
		Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(int32_t data) {
		data = zton(data);
		Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(uint32_t data) {
		data = zton(data);
		Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(int64_t data) {
		data = zton(data);
		Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(uint64_t data) {
		data = zton(data);
		Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(float data) {
		Append({ &data, sizeof(data) });
		return *this;
	}

	Packet& Packet::operator<<(double data) {
		Append({ &data, sizeof(data) });
		return *this;
	}
}