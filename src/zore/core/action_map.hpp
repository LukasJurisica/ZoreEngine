#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/events.hpp"
#include <unordered_map>
#include <functional>

//========================================================================
//	Action Map Class
//========================================================================

namespace zore {

	class ActionMap {
	public:
		enum class Source { INTERNAL, KEYBOARD, MOUSE, CONTROLLER };

		struct Key {
			Key(Source source, uint32_t code, uint32_t type);
			operator uint32_t() { return data; }
			Source GetSource();
			uint32_t GetCode();
			uint32_t GetType();

			uint32_t data;
		};

	public:
		ActionMap();
		~ActionMap() = default;

		template <typename ClassType>
		void Register(Source source, uint32_t code, bool on_start, bool on_stop, void(ClassType::* action)(Key), ClassType* obj) {
			Register(source, code, on_start, on_stop, std::bind(action, obj, std::placeholders::_1));
		}
		void Register(Source source, uint32_t code, bool on_start, bool on_stop, std::function<void(Key)> action);
		void Clear(Source source, uint32_t code, bool on_start, bool on_stop);
		void SetActive(bool value);

		bool OnButtonPress(const ButtonPressedEvent& e);
		bool OnButtonRelease(const ButtonReleasedEvent& e);
		bool OnKeyPress(const KeyPressedEvent& e);
		bool OnKeyRelease(const KeyReleasedEvent& e);
		bool OnMousePress(const MousePressedEvent& e);
		bool OnMouseRelease(const MouseReleasedEvent& e);

	private:
		bool HandleEvent(Source source, uint32_t code, bool start) const;

	private:
		bool m_active;
		std::unordered_map<uint32_t, std::function<void(Key)>> m_actions;
		event::MultiHandler m_handler;
	};
}