#pragma once

#include "zore/ui/Layer.hpp"
#include "zore/core/ActionMap.hpp"

namespace zore::UI {

    //========================================================================
    //	UI Manager Class
    //========================================================================

    class Manager {
    public:
        static Layer& CreateLayer(const std::string& name, const std::string& style = "");
        static Layer* Bind(Layer& layer);
        static Layer* Bind(const std::string& name);
        static void Bind(ActionMap& action_map);

    private:

    };
}