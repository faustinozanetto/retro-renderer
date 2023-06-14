#pragma once

#include <imgui.h>

namespace retro::editor
{
    class editor_panel
    {
    public:
        virtual ~editor_panel() = default;

        virtual void on_render_panel() = 0;
    };
}