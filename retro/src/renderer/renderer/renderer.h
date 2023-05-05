#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace retro::renderer
{
    struct renderer_data
    {
        GLFWwindow *window_handle;
        glm::vec4 clear_color;
        bool vsync_enabled;
    };

    class renderer
    {
    public:
        static void initialize();

        static bool get_window_should_close();
        static void clear_screen();
        static void poll_input();
        static void swap_buffers();
        static void set_clear_color();
        static void set_vsync_enabled(bool is_vsync_enabled);
    };
}
