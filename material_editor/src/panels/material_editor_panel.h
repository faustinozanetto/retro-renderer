#pragma once

#include <imgui.h>

namespace retro::material_editor
{
    class material_editor_panel
    {
    public:
        virtual ~material_editor_panel() = default;

        virtual void on_render_panel() = 0;
    };
}