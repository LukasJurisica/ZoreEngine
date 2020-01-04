#include "core/Event.hpp"

//////////////////
// Mouse Events //
//////////////////

MouseMoveEvent::MouseMoveEvent (float x, float y) : m_mouseX(x), m_mouseY(y) {}

MouseButtonEvent::MouseButtonEvent(int button, int action) : m_button(button), m_action(action) {}

////////////////
// Key Events //
////////////////

keyboardEvent::keyboardEvent(int keycode, int action) : m_keycode(keycode), m_action(action) {}

///////////////////
// Window Events //
///////////////////

WindowResizeEvent::WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}