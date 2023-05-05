#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace retro::renderer
{
    struct window_data
    {
        int width;
        int height;
        std::string title;
    };

    class window
    {
    public:
        window(int width, int height, std::string title);
        ~window();

    private:
        void initialize();

        window_data m_data;
        GLFWwindow *m_handle;
    };
}