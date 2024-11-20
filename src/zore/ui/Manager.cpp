#include "zore/ui/Manager.hpp"
#include "zore/Devices.hpp"
#include "zore/Debug.hpp"

namespace zore::UI {

    static std::unordered_map<std::string, Layer> s_layers;
    static Layer* s_active_layer = nullptr;
    static ActionMap* s_active_action_map = nullptr;

    //========================================================================
    //	UI Manager Class
    //========================================================================

    Layer& Manager::CreateLayer(const std::string& name, const std::string& style) {
        return s_layers.emplace(name, style).first->second;
    }

    Layer* Manager::Bind(Layer& layer) {
        s_active_layer = &layer;
		return s_active_layer;
    }

    Layer* Manager::Bind(const std::string& name) {
		auto iter = s_layers.find(name);
		s_active_layer = (iter != s_layers.end()) ? &iter->second : nullptr;
        return s_active_layer;
    }

    void Manager::Bind(ActionMap& action_map) {
        s_active_action_map = &action_map;
    }

    //========================================================================
    //	UI Event Listener Class
    //========================================================================

    static int s_temp = 0;

    class EventListener : Window::Listener, Keyboard::Listener, Mouse::Listener {
    public:
        EventListener() : Keyboard::Listener(100), Mouse::Listener(100) {}
        ~EventListener() = default;

        void OnWindowResize(int width, int height, float aspect_ratio) override {
            if (!s_active_layer)
                return;
            s_active_layer->Resize(width, height);
        }

        bool OnMouseMove(float x, float y, float dx, float dy) override {
            return false;
        }

        bool OnMousePress(int button) override {
            return false;
        }

        bool OnMouseRelease(int button) override {
            return false;
        }

        bool OnKeyPress(int button, int mods) override {
            return false;
        }

        bool OnKeyRelease(int button) override {
            return false;
        }
    };

    static EventListener s_event_listener;
}