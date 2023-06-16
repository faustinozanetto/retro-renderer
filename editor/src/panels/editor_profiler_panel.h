#pragma once

#include "editor_panel.h"

namespace retro::editor
{
    class editor_profiler_panel : public editor_panel
    {
    public:
        editor_profiler_panel();
        ~editor_profiler_panel() override;

        void on_render_panel() override;
    };
}