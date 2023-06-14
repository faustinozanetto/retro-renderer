#pragma once

#include "editor_panel.h"

namespace retro::editor
{
    class editor_viewport_panel : public editor_panel
    {
    public:
        editor_viewport_panel();
        ~editor_viewport_panel() override;

        void on_render_panel() override;
    };
}