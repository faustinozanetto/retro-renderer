#pragma once

#include "material_editor_panel.h"

namespace retro::material_editor
{
    class material_details_panel : public material_editor_panel
    {
    public:
        material_details_panel();
        ~material_details_panel() override;

        void on_render_panel() override;

    private:
    };
}