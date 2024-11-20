#pragma once

#include <zore/utils/DataTypes.hpp>
#include <unordered_map>

//========================================================================
//	Action Map Class
//========================================================================

class ActionMap {
public:
	enum class Source { INTERNAL, KEYBOARD, MOUSE, GAMEPAD };

public:
	ActionMap() = default;
	~ActionMap() = default;

	void RegisterAction(Source source, uint32_t code, bool on_start, bool on_stop, void (*action)(bool));
	bool HandleEvent(Source source, uint32_t code, bool start) const;

	static Source GetSource(uint32_t key);
	static uint32_t GetCode(uint32_t key);
	static uint32_t GetType(uint32_t key);

private:
	static uint32_t CreateSourceEventKey(Source source, uint32_t code, uint32_t type);

private:
	std::unordered_map<uint32_t, void (*)(bool)> m_actions;
};