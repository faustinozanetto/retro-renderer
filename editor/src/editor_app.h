#pragma once

#include "editor_main_layer.h"

#include <retro.h>

namespace retro::editor
{
    class editor_app : public core::application
    {
    public:
        editor_app();
        ~editor_app() override;

        void initialize();

        void on_update() override;

        void on_handle_event(events::base_event &event) override;
        bool on_window_resize(events::window_resize_event &resize_event) override;
        bool on_key_pressed(events::key_pressed_event &key_pressed_event);

    private:
        std::shared_ptr<editor_main_layer> m_main_layer;
    };
}