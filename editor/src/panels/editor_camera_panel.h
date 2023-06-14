#pragma once

#include "editor_panel.h"

namespace retro::editor
{
    class editor_camera_panel : public editor_panel
    {
    public:
        editor_camera_panel();
        ~editor_camera_panel() override;

        void on_render_panel() override;
    };
}