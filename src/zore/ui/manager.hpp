#pragma once

#include "zore/ui/layer.hpp"
#include "zore/core/action_map.hpp"

namespace zore::UI {

    //========================================================================
    //	UI Manager Class
    //========================================================================

    class Manager {
    public:
        static Layer& CreateLayer(const std::string& name);
        static Layer* Bind(const std::string& name);
        static void Bind(ActionMap& action_map);

        virtual void OnLayerChange(Layer* layer, const std::string& name) {};

    protected:
		Manager();
		~Manager() = default;
    };
}