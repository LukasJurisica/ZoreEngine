#include "zore/ui/manager.hpp"
#include "zore/events.hpp"
#include "zore/debug.hpp"

namespace zore::UI {

	static std::unordered_map<std::string, UNIQUE<Layer>> s_layers;

	//========================================================================
	//	UI Event Listener Class
	//========================================================================

	class EventListener {
	public:
		EventListener() {
			event::MultiHandler& handler = event::MultiHandler::Get();
			handler.Register(&OnMouseMove, 100);
			handler.Register(&OnMousePress, 100);
			handler.Register(&OnMouseRelease, 100);
			handler.Register(&OnKeyPress, 100);
			handler.Register(&OnWindowResize, 100);
		}
		~EventListener() = default;

		static bool OnMouseMove(const MouseMovedEvent& e) {
			Layer::HandleMouseMove(static_cast<uint32_t>(e.x), static_cast<uint32_t>(e.y));
			return false;
		}

		static bool OnMousePress(const MousePressedEvent& e) {
			return HandleButton(Layer::HandleMousePress(e.button), true);
		}

		static bool OnMouseRelease(const MouseReleasedEvent& e) {
			return HandleButton(Layer::HandleMouseRelease(e.button), false);
		}

		static bool OnKeyPress(const KeyPressedEvent& e) {
			return HandleButton(Layer::HandleKeyPress(e.key), true);
		}

		static bool HandleButton(uint32_t button_id, bool press) {
			if (button_id != uuid_32::INVALID_ID) {
				if (press)
					event::Manager::Dispatch(ButtonPressedEvent(button_id));
				else
					event::Manager::Dispatch(ButtonReleasedEvent(button_id));
			}
			return button_id != uuid_32::INVALID_ID;
		}

		static bool OnWindowResize(const WindowResizedEvent& e) {
			Layer::Resize(e.width, e.height);
			return false;
		}
	};

	//========================================================================
	//	UI Manager Class
	//========================================================================

	static Manager* s_manager_instance = nullptr;

	Layer& Manager::CreateLayer(const std::string& name) {
		static EventListener s_event_listener;
		s_layers[name] = MAKE_UNIQUE<Layer>();
		return *s_layers[name];
	}

	Layer* Manager::Bind(const std::string& name) {
		auto iter = s_layers.find(name);
		Layer* layer = (iter != s_layers.end()) ? iter->second.get() : nullptr;
		if (layer) {
			layer->Bind();
			if (s_manager_instance)
				s_manager_instance->OnLayerChange(layer, name);
		}
		else
			Logger::Warn("Attempted to bind layer with name that does not exist:", name);
		return layer;
	}

	Manager::Manager() {
		ENSURE(!s_manager_instance, "Attempted to create multiple UI Managers - this is not supported.");
		Logger::Info("UI Manager Initialized");
		s_manager_instance = this;
	}
}