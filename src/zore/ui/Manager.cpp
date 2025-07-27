#include "zore/ui/Manager.hpp"
#include "zore/Events.hpp"
#include "zore/Debug.hpp"

namespace zore::UI {

	static std::unordered_map<std::string, UNIQUE<Layer>> s_layers;
	static ActionMap* s_active_action_map = nullptr;

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
			uint32_t trigger = Layer::HandleMousePress(e.button);
			if (trigger && s_active_action_map)
				return s_active_action_map->HandleEvent(ActionMap::Source::INTERNAL, trigger, 1);
			return false;
		}

		static bool OnMouseRelease(const MouseReleasedEvent& e) {
			uint32_t trigger = Layer::HandleMouseRelease(e.button);
			if (trigger && s_active_action_map)
				return s_active_action_map->HandleEvent(ActionMap::Source::INTERNAL, trigger, 0);
			return false;
		}

		static bool OnKeyPress(const KeyPressedEvent& e) {
			uint32_t trigger = Layer::HandleKeyPress(e.key);
			if (trigger && s_active_action_map)
				return s_active_action_map->HandleEvent(ActionMap::Source::INTERNAL, trigger, 1);
			return false;
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

	void Manager::Bind(ActionMap& action_map) {
		s_active_action_map = &action_map;
	}

	Manager::Manager() {
		ENSURE(!s_manager_instance, "Attempted to create multiple UI Managers - this is not supported.");
		Logger::Info("UI Manager Initialized");
		s_manager_instance = this;
	}
}