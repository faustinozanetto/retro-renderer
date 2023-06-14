#pragma once

#include "../../editor_panel.h"

namespace retro::editor
{
    class editor_actor_transform_component_panel : public editor_panel
    {
    public:
        editor_actor_transform_component_panel();
        ~editor_actor_transform_component_panel() override;

        void on_render_panel() override;
    };
}