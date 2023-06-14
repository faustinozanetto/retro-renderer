#pragma once

#include "../editor_panel.h"

namespace retro::editor
{
    class editor_scene_hierarchy_panel : public editor_panel
    {
    public:
        editor_scene_hierarchy_panel();
        ~editor_scene_hierarchy_panel() override;

        void on_render_panel() override;
    };
}