#pragma once

#include "editor_panel.h"

namespace retro::editor
{
    class editor_physics_panel : public editor_panel
    {
    public:
        editor_physics_panel();
        ~editor_physics_panel() override;

        void on_render_panel() override;
    };
}