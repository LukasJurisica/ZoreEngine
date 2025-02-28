#pragma once

#include "zore/events/Event.hpp"

#include <vector>

namespace zore {

	struct WindowResizedEvent : public Event::EventBase {
		WindowResizedEvent(int width, int height) : width(width), height(height) {
			aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
		}
		int width, height;
		float aspect_ratio;
	};

	struct WindowMovedEvent : public Event::EventBase {
		WindowMovedEvent(int xpos, int ypos) : xpos(xpos), ypos(ypos) {}
		int xpos, ypos;
	};

	struct WindowFocusedEvent : public Event::EventBase {
		WindowFocusedEvent(bool focused) : focused(focused) {}
		bool focused;
	};

	struct WindowMaximizedEvent : public Event::EventBase {
		WindowMaximizedEvent(bool maximized) : maximized(maximized) {}
		bool maximized;
	};

	struct WindowMinimizedEvent : public Event::EventBase {
		WindowMinimizedEvent(bool minimized) : minimized(minimized) {}
		bool minimized;
	};

	struct WindowFileDropEvent : public Event::EventBase {
		WindowFileDropEvent(const std::vector<std::string>& files_paths) : paths(paths) {}
		WindowFileDropEvent(int count, const char** files_paths) {
			paths.reserve(count);
			for (int i = 0; i < count; i++)
				paths.emplace_back(files_paths[i]);
		}
		std::vector<std::string> paths;
	};
}