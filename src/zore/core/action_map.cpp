#include "action_map.hpp"
#include "zore/utils/bits.hpp"

namespace zore {

	//========================================================================
	//	Action Map Key
	//========================================================================

	ActionMap::Key::Key(Source source, uint32_t code, uint32_t type) {
		data = (code << 3) | (type << 2) | static_cast<uint32_t>(source);
	}

	ActionMap::Source ActionMap::Key::GetSource() {
		return static_cast<Source>(GET_N_BITS(data, 2, 0));
	}

	uint32_t ActionMap::Key::GetCode() {
		return GET_N_BITS(data, 29, 3);
	}

	uint32_t ActionMap::Key::GetType() {
		return GET_BIT(data, 2);
	}

	//========================================================================
	//	Action Map Logic
	//========================================================================

	ActionMap::ActionMap() : m_active(true) {
		m_handler.Register(&ActionMap::OnButtonPress, this);
		m_handler.Register(&ActionMap::OnButtonRelease, this);
		m_handler.Register(&ActionMap::OnKeyPress, this);
		m_handler.Register(&ActionMap::OnKeyRelease, this);
		m_handler.Register(&ActionMap::OnMousePress, this);
		m_handler.Register(&ActionMap::OnMouseRelease, this);
	}

	void ActionMap::Register(Source source, uint32_t code, bool on_start, bool on_stop, std::function<void(Key)> action) {
		if (on_start)
			m_actions[Key(source, code, 1u)] = action;
		if (on_stop)
			m_actions[Key(source, code, 0u)] = action;
	}

	void ActionMap::Clear(Source source, uint32_t code, bool on_start, bool on_stop) {
		if (on_start)
			m_actions.erase(Key(source, code, 1u));
		if (on_stop)
			m_actions.erase(Key(source, code, 0u));
	}

	void ActionMap::SetActive(bool value) {
		m_active = value;
	}

	bool ActionMap::OnButtonPress(const ButtonPressedEvent& e) {
		return HandleEvent(Source::INTERNAL, e.uuid, true);
	}

	bool ActionMap::OnButtonRelease(const ButtonReleasedEvent& e) {
		return HandleEvent(Source::INTERNAL, e.uuid, false);
	}

	bool ActionMap::OnKeyPress(const KeyPressedEvent& e) {
		return HandleEvent(Source::KEYBOARD, e.key, true);
	}

	bool ActionMap::OnKeyRelease(const KeyReleasedEvent& e) {
		return HandleEvent(Source::KEYBOARD, e.key, false);
	}

	bool ActionMap::OnMousePress(const MousePressedEvent& e) {
		return HandleEvent(Source::MOUSE, e.button, true);
	}

	bool ActionMap::OnMouseRelease(const MouseReleasedEvent& e) {
		return HandleEvent(Source::MOUSE, e.button, false);
	}

	bool ActionMap::HandleEvent(Source source, uint32_t code, bool start) const {
		if (m_active == false)
			return false;
		auto it = m_actions.find(Key(source, code, static_cast<uint32_t>(start)));
		if (it != m_actions.end()) {
			it->second(Key(source, code, start));
			return true;
		}
		return false;
	}
}