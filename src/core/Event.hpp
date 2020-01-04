#pragma once

#include "window/InputCodes.hpp"
#include <functional>

// Event Class
class Event {
public:
	enum class EventType {
		none,
		mouseMove,
		mouseButton,
		keyboard,
		windowResize,
		scroll
	};
	virtual EventType getEventType() const = 0;
};

// Mouse Move Event Class
class MouseMoveEvent : public Event {
public:
	MouseMoveEvent(float x, float y);
	inline EventType getEventType() const override { return m_eventType; }
	inline float getMouseX() const { return m_mouseX; }
	inline float getMouseY() const { return m_mouseY; }

private:
	const static EventType m_eventType = EventType::mouseMove;
	float m_mouseX, m_mouseY;
};

// Mouse Press/Release Event Class
class MouseButtonEvent : public Event {
public:
	MouseButtonEvent(int button, int action);
	inline EventType getEventType() const override { return m_eventType; }
	inline int getButton() const { return m_button; }
	inline int getAction() const { return m_action; }

private:
	const static EventType m_eventType = EventType::mouseButton;
	int m_button, m_action;
};

// Key Press/Release Event Class
class keyboardEvent : public Event {
public:
	keyboardEvent(int keycode, int action);
	inline EventType getEventType() const override { return m_eventType; }
	inline int getKeycode() const { return m_keycode; }
	inline int getAction() const { return m_action; }

private:
	const static EventType m_eventType = EventType::keyboard;
	int m_keycode, m_action;
};

// Window Resize Event Class
class WindowResizeEvent : public Event {
public:
	WindowResizeEvent(int width, int height);
	inline EventType getEventType() const override { return m_eventType; }
	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }

private:
	const static EventType m_eventType = EventType::windowResize;
	int m_width, m_height;
};