#pragma once

#include "panels/material_editor_panel.h"

#include <retro.h>

namespace retro::material_editor
{
    class material_editor_app : public core::application
    {
    public:
        material_editor_app();
        ~material_editor_app() override;

        void on_update() override;

        void on_handle_event(retro::events::base_event &event) override;
        bool on_window_resize(retro::events::window_resize_event &resize_event) override;

    private:
        std::vector<std::shared_ptr<material_editor_panel>> m_editor_panels;
    };
}