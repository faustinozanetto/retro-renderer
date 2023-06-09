#pragma once

#include "iostream"
#include "events/base_event.h"
#include "events/event_dispatcher.h"
#include "events/window_events.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace retro::renderer
{
    struct window_data
    {
        int width;
        int height;
        std::string title;
        std::function<void(events::base_event &)> event_func;
    };

    struct window_specification {
		std::string title;
		int width;
		int height;
    };

    class window
    {
    public:
        window(const window_specification& window_specification);
        ~window();

        /* Getters */
        int get_width() const { return m_data.width; }
        int get_height() const { return m_data.height; }
        std::string get_title() const { return m_data.title; }
        GLFWwindow *get_handle() const { return m_handle; }

        void set_event_function(const std::function<void(events::base_event &)> func);

    private:
        void initialize();
        void setup_callbacks();

        window_data m_data;
        GLFWwindow *m_handle;
    };
}