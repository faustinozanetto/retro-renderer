#pragma once

#include "renderer/window/window.h"

// Entry point used in applications.
int main(int argc, char **argv);

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

        virtual void on_update() = 0;
        void on_event(events::base_event &event);

    private:
        void main_loop();

        bool on_window_resize(events::window_resize_event &resize_event);

        std::shared_ptr<renderer::window> m_window;

        friend int ::main(int argc, char **argv);
        static application *s_instance;
    };

    application *create_application();
}
