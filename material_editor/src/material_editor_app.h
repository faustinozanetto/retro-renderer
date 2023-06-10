#pragma once

#include "panels/material_editor_panel.h"
#include "preview/material_preview.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#include <retro.h>
#include <memory>

namespace retro::material_editor
{
    class material_editor_app : public core::application
    {
    public:
        material_editor_app();
        ~material_editor_app() override;

        /* Getters */
        const std::shared_ptr<renderer::material>&get_current_material() const { return m_current_material; }
        const std::shared_ptr<material_preview> &get_material_preview() const { return m_material_preview; }

        /* Functions */
        void on_update() override;

        void on_open_material_file();

        void initialize();

        void on_handle_event(retro::events::base_event &event) override;
        bool on_window_resize(retro::events::window_resize_event &resize_event) override;

    private:
        std::shared_ptr<material_preview> m_material_preview;
        std::vector<std::shared_ptr<material_editor_panel>> m_editor_panels;
        std::shared_ptr<renderer::material> m_current_material;
    };
}