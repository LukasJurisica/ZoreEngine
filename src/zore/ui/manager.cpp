#include "zore/ui/manager.hpp"
#include "zore/devices/window.hpp"
#include "zore/events.hpp"
#include "zore/structures/string_unordered_map.hpp"
#include <algorithm>
#include <memory>

namespace zore::UI {

	static zore::string_unordered_map<std::unique_ptr<Layer>> s_layers;
	static Layer* s_active_layer = nullptr;

	class EventListener {
	public:
		EventListener() {
			event::MultiHandler::Get().Register(&OnMouseMove, 100);
			event::MultiHandler::Get().Register(&OnMousePress, 100);
			event::MultiHandler::Get().Register(&OnMouseRelease, 100);
			event::MultiHandler::Get().Register(&OnWindowResize, 100);
		}

		static bool OnMouseMove(const MouseMovedEvent& event) {
			if (s_active_layer)
				s_active_layer->HandleMouseMove(static_cast<int32_t>(event.x), static_cast<int32_t>(event.y));
			return false;
		}

		static bool OnMousePress(const MousePressedEvent& event) {
			if (!s_active_layer) return false;
			const Element::ID id = s_active_layer->HandleMousePress(static_cast<uint32_t>(event.button));
			if (id == uint32_max) return false;
			event::Manager::Dispatch(ButtonPressedEvent(id));
			return true;
		}

		static bool OnMouseRelease(const MouseReleasedEvent& event) {
			if (!s_active_layer) return false;
			const Element::ID id = s_active_layer->HandleMouseRelease(static_cast<uint32_t>(event.button));
			if (id == uint32_max) return false;
			event::Manager::Dispatch(ButtonReleasedEvent(id));
			return true;
		}

		static bool OnWindowResize(const WindowResizedEvent& event) {
			Manager::Resize(static_cast<uint32_t>(event.width), static_cast<uint32_t>(event.height));
			return false;
		}
	};

	Layer& Manager::CreateLayer(std::string_view name) {
		static EventListener listener;
		auto layer = std::make_unique<Layer>();
		Layer& result = *layer;
		s_layers.insert_or_assign(std::string(name), std::move(layer));
		if (!s_active_layer)
			Bind(name);
		return result;
	}

	Layer* Manager::GetLayer(std::string_view name) {
		auto it = s_layers.find(name);
		return it == s_layers.end() ? nullptr : it->second.get();
	}

	Layer* Manager::Bind(std::string_view name) {
		Layer* layer = GetLayer(name);
		if (!layer)
			return nullptr;
		s_active_layer = layer;
		const zm::ivec2& size = Window::GetSize();
		Resize(static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y));
		return layer;
	}

	Layer* Manager::GetActiveLayer() {
		return s_active_layer;
	}

	void Manager::Resize(uint32_t width, uint32_t height) {
		if (!s_active_layer)
			return;
		s_active_layer->Resize(width, height);
		s_active_layer->Flush();
	}

	void Manager::Clear() {
		s_active_layer = nullptr;
		s_layers.clear();
	}
}
