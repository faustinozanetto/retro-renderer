#pragma once

#include "editor_panel.h"

namespace retro::editor
{
    class editor_toolbar_panel : public editor_panel
    {
    public:
        editor_toolbar_panel();
        ~editor_toolbar_panel() override;

        void on_render_panel() override;
    };
}