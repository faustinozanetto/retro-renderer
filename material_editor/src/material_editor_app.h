#pragma once

#include "panels/material_editor_panel.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#include <retro.h>

namespace retro::material_editor
{
    class material_editor_app : public core::application
    {
    public:
        material_editor_app();
        ~material_editor_app() override;

        /* Getters */
        const std::string& get_current_material_file_path() const { return m_current_material_file_path; }

        /* Functions */
        void on_update() override;

        void on_open_material_file();

        void on_handle_event(retro::events::base_event &event) override;
        bool on_window_resize(retro::events::window_resize_event &resize_event) override;

    private:
        std::vector<std::shared_ptr<material_editor_panel>> m_editor_panels;
        std::string m_current_material_file_path;
    };
}