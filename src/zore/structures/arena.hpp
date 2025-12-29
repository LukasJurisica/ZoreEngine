#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <cstring>

namespace zore {

	//========================================================================
	//	Arena
	//=========================================================================

	template <typename T, typename S = size_t>
	class Arena {
	public:
		struct Region {
			S offset;
			S size;
		};

	public:
		Arena(S size = 0) {
			if (size > 0) {
				m_data.resize(size);
				m_free_regions.emplace_back(0, size);
			}
		}

		Arena(const Arena&) = delete;
		Arena& operator=(const Arena&) = delete;
		~Arena() = default;

		S Allocate(S size) {
			for (auto it = m_free_regions.rbegin(); it != m_free_regions.rend(); ++it) {
				Region r = *it;
				if (r.size >= size) {
					m_free_regions.erase(std::next(it).base());
					if (r.size > size)
						Free(r.offset + size, r.size - size);
					return r.offset;
				}
			}
			return Grow(size);
		}

		S Reallocate(S old_offset, S old_size, S new_size) {
			if (new_size == old_size)
				return old_offset;
			if (new_size < old_size) {
				Free(old_offset + new_size, old_size - new_size);
				return old_offset;
			}

			for (auto it = m_free_regions.begin(); it != m_free_regions.end(); ++it) {
				if (it->offset == old_offset + old_size) {
					if (new_size <= it->size + old_size) {
						S grow = new_size - old_size;
						it->offset += grow;
						it->size -= grow;
						if (it->size == 0)
							m_free_regions.erase(it);
						return old_offset;
					}
					break;
				}
			}

			S new_offset = Allocate(new_size);
			std::memcpy(&m_data[new_offset], &m_data[old_offset], old_size * sizeof(T));
			Free(old_offset, old_size);
			return new_offset;
		}

		void Free(S offset, S size) {
			S begin = offset;
			S end = offset + size;

			for (auto it = m_free_regions.begin(); it != m_free_regions.end(); ) {
				if (it->offset + it->size == begin) {
					begin = it->offset;
					it = m_free_regions.erase(it);
				}
				else if (it->offset == end) {
					end = it->offset + it->size;
					it = m_free_regions.erase(it);
				}
				else {
					++it;
				}
			}
			m_free_regions.push_back({ begin, end - begin });
		}

		T& operator[](S offset) {
			return m_data[offset];
		}

		const T& operator[](S offset) const {
			return m_data[offset];
		}

	private:
		S Grow(S size) {
			S offset = static_cast<S>(m_data.size());
			m_data.resize(offset + size);
			return offset;
		}

	private:
		std::vector<T> m_data;
		std::vector<Region> m_free_regions;
	};
}
