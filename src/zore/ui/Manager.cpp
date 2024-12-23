#include "zore/ui/Manager.hpp"
#include "zore/Devices.hpp"
#include "zore/Debug.hpp"

namespace zore::UI {

    static std::unordered_map<std::string, Layer> s_layers;
    static ActionMap* s_active_action_map = nullptr;

    //========================================================================
    //	UI Event Listener Class
    //========================================================================

    class EventListener : Window::Listener, Keyboard::Listener, Mouse::Listener {
    public:
        EventListener() : Keyboard::Listener(100), Mouse::Listener(100) {}
        ~EventListener() = default;

        void OnWindowResize(int width, int height, float aspect_ratio) override {
            Layer::Resize(width, height);
        }

        bool OnMouseMove(float x, float y, float dx, float dy) override {
			Layer::HandleMouseMove(static_cast<uint32_t>(x), static_cast<uint32_t>(y));
            return false;
        }

        bool OnMousePress(int button) override {
			uint32_t trigger = Layer::HandleMousePress(button);
			if (trigger && s_active_action_map)
				return s_active_action_map->HandleEvent(ActionMap::Source::INTERNAL, trigger, 1);
            return false;
        }

        bool OnMouseRelease(int button) override {
			uint32_t trigger = Layer::HandleMouseRelease(button);
			if (trigger && s_active_action_map)
                return s_active_action_map->HandleEvent(ActionMap::Source::INTERNAL, trigger, 0);
            return false;
        }

        bool OnKeyPress(int key, int mods) override {
			uint32_t trigger = Layer::HandleKeyPress(key);
			if (trigger && s_active_action_map)
                return s_active_action_map->HandleEvent(ActionMap::Source::INTERNAL, trigger, 1);
            return false;
        }
    };

    //========================================================================
    //	UI Manager Class
    //========================================================================

    static Manager* s_manager_instance = nullptr;

    Layer& Manager::CreateLayer(const std::string& name) {
        static EventListener s_event_listener;
		return s_layers.emplace(name, Layer()).first->second;
    }

    Layer* Manager::Bind(const std::string& name) {
		auto iter = s_layers.find(name);
        Layer* layer = (iter != s_layers.end()) ? &iter->second : nullptr;
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
		s_manager_instance = this;
    }
}