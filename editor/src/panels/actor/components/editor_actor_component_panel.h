#pragma once

#include "../../editor_panel.h"
#include "../../../editor_main_layer.h"
#include "../../../utils/editor_ui_utils.h"

#include <string>

namespace retro::editor
{
    class editor_actor_component_panel : public editor_panel
    {
    public:
        editor_actor_component_panel(const std::string &component_name);
        ~editor_actor_component_panel() override;

        void on_render_panel() override;
        
        virtual std::pair<bool, size_t> get_actor_component_details() = 0;
        virtual void on_render_component_details() = 0;
        virtual void on_remove_component() = 0;

    protected:
        std::string m_component_name;
    };

}