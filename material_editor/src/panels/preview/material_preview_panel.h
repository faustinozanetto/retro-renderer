#pragma once

#include "../material_editor_panel.h"

namespace retro::material_editor
{
    class material_preview_panel : public material_editor_panel
    {
    public:
        material_preview_panel();
        ~material_preview_panel() override;

        void on_render_panel() override;

    private:
    };
}