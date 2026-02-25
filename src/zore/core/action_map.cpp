#include "action_map.hpp"
#include "zore/utils/bits.hpp"

namespace zore {

	void ActionMap::RegisterAction(Source source, uint32_t code, bool on_start, bool on_stop, void (*action)(bool)) {
		if (on_start)
			m_actions[CreateSourceEventKey(source, code, 1)] = action;
		if (on_stop)
			m_actions[CreateSourceEventKey(source, code, 0)] = action;
	}

	bool ActionMap::HandleEvent(Source source, uint32_t code, bool start) const {
		auto iter = m_actions.find(CreateSourceEventKey(source, code, start ? 1 : 0));
		if (iter == m_actions.end())
			return false; // No Action Required
		iter->second(start);
		return true;
	}

	ActionMap::Source ActionMap::GetSource(uint32_t key) {
		return static_cast<Source>(GET_N_BITS(key, 2, 0));
	}

	uint32_t ActionMap::GetCode(uint32_t key) {
		return GET_N_BITS(key, 29, 3);
	}

	uint32_t ActionMap::GetType(uint32_t key) {
		return GET_N_BITS(key, 1, 2);
	}

	uint32_t ActionMap::CreateSourceEventKey(Source source, uint32_t code, uint32_t type) {
		// 29 bits code, 1 bit type, 2 bits source
		return (code << 3) | (type << 2) | static_cast<uint32_t>(source);
	}
}