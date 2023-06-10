#pragma once

#include "../material_editor_panel.h"

#include <retro.h>

namespace retro::material_editor
{
    struct material_previw_panel_model {
        std::string label;
        std::shared_ptr<renderer::model> model;
    };

    class material_preview_panel : public material_editor_panel
    {
    public:
        material_preview_panel();
        ~material_preview_panel() override;

        void on_render_panel() override;

        void setup_preview_models();

    private:
        bool m_rotate_model;
        std::vector<material_previw_panel_model> m_preview_models;
    };
}