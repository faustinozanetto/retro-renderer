#pragma once

#include "editor_panel.h"

namespace retro::editor
{
    class editor_renderer_panel : public editor_panel
    {
    public:
        editor_renderer_panel();
        ~editor_renderer_panel() override;

        void on_render_panel() override;
    };
}