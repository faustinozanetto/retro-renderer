#include "rtpch.h"
#include "window.h"

namespace retro::renderer
{
    window::window(int width, int height, const std::string& title)
    {
        m_data.width = width;
        m_data.height = height;
        m_data.title = title;

        initialize();
    }

    window::~window()
    {
    }

    void window::initialize()
    {
        const int glfwInitializeState = glfwInit();
        if (glfwInitializeState == GLFW_FALSE)
            assert(false);

        // GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        // Window handle creation
        m_handle = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);

        // Store data pointer
        glfwSetWindowUserPointer(m_handle, &m_data);
    }
}
