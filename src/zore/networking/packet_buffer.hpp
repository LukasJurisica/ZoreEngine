#pragma once

namespace zore::net {

	class PacketBuffer {
	public:
		PacketBuffer(int size) : m_size(size), m_data(new char[size]) {}

		PacketBuffer(const PacketBuffer&) = delete;

		PacketBuffer(PacketBuffer&& other) noexcept : m_size(other.m_size), m_data(other.m_data) {
			other.m_size = 0;
			other.m_data = nullptr;
		}

		PacketBuffer& operator=(const PacketBuffer&) = delete;

		PacketBuffer& operator=(PacketBuffer&& other) noexcept {
			if (this != &other)
				Move(other);
			return *this;
		}

		~PacketBuffer() {
			delete[] m_data;
		}

		void Move(PacketBuffer& other) {
			delete[] m_data;
			m_size = other.m_size;
			m_data = other.m_data;
			other.m_size = 0;
			other.m_data = nullptr;
		}

		operator char* () {
			return m_data;
		}

		operator unsigned char* () {
			return reinterpret_cast<unsigned char*>(m_data);
		}

		int size() const {
			return m_size;
		}

		void* data() {
			return m_data;
		}

	private:
		int m_size;
		char* m_data;
	};
}