#pragma once

#include "iostream"

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
        window(int width, int height, const std::string &title);
        ~window();

        /* Getters */
        int get_width() const { return m_data.width; }
        int get_height() const { return m_data.height; }
        std::string get_title() const { return m_data.title; }
        GLFWwindow *get_handle() const { return m_handle; }

    private:
        void initialize();

        window_data m_data;
        GLFWwindow *m_handle;
    };
}