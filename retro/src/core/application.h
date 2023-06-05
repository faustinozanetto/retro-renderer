﻿#pragma once

#include "audio/audio_context.h"
#include "renderer/window/window.h"

// Entry point used in applications.
int main(int argc, char **argv);

#define BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace retro::core
{
    class application
    {
    public:
        application(const std::string &working_directory);
        virtual ~application();

        /* Getters */
        static application &get() { return *s_instance; }
        std::shared_ptr<renderer::window> &get_window() { return m_window; }

        void on_event(events::base_event &event);

        virtual void on_update() = 0;
        virtual void on_handle_event(events::base_event &event) = 0;
        virtual bool on_window_resize(events::window_resize_event &resize_event);

    private:
        void main_loop();

        std::shared_ptr<renderer::window> m_window;
        std::shared_ptr<audio::audio_context> m_audio_context;
        float m_frame_delay;

        friend int ::main(int argc, char **argv);
        static application *s_instance;
    };

    application *create_application();
}
