#pragma once

#include "zore/events/event.hpp"
#include <vector>
#include <string>

namespace zore {

	struct WindowResizedEvent : public event::Base {
		WindowResizedEvent(int width, int height) : width(width), height(height) {
			aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
		}
		int width, height;
		float aspect_ratio;
	};

	struct WindowMovedEvent : public event::Base {
		WindowMovedEvent(int xpos, int ypos) : xpos(xpos), ypos(ypos) {}
		int xpos, ypos;
	};

	struct WindowFocusedEvent : public event::Base {
		WindowFocusedEvent(bool focused) : focused(focused) {}
		bool focused;
	};

	struct WindowMaximizedEvent : public event::Base {
		WindowMaximizedEvent(bool maximized) : maximized(maximized) {}
		bool maximized;
	};

	struct WindowMinimizedEvent : public event::Base {
		WindowMinimizedEvent(bool minimized) : minimized(minimized) {}
		bool minimized;
	};

	struct WindowFileDropEvent : public event::Base {
		WindowFileDropEvent(const std::vector<std::string>& file_paths) : paths(file_paths) {}
		WindowFileDropEvent(int count, const char** file_paths) {
			paths.reserve(count);
			for (int i = 0; i < count; i++)
				paths.emplace_back(file_paths[i]);
		}
		std::vector<std::string> paths;
	};
}