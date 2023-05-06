#include "rtpch.h"
#include "window.h"

namespace retro::renderer
{
    window::window(int width, int height, const std::string &title)
    {
        RT_TRACE("Retro Renderer | Window Initialization");
        RT_TRACE("   - Title: {0}", title);
        RT_TRACE("   - Width: {0}", width);
        RT_TRACE("   - Height: {0}", height);
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
        RT_ASSERT_MSG(glfwInitializeState != GLFW_FALSE, "Could not initialize GLFW!");

        // GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        // Window handle creation
        m_handle = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);

        // Store data pointer
        glfwSetWindowUserPointer(m_handle, &m_data);

        setup_callbacks();

        RT_TRACE("Retro Renderer | Window initialization completed.");
    }

    void window::setup_callbacks()
    {
        // 1. Resize callback
        glfwSetWindowSizeCallback(m_handle, [](GLFWwindow *native_window, int width, int height)
                                  {
            window_data &data = *static_cast<window_data *>(glfwGetWindowUserPointer(native_window));
            data.width = width;
            data.height = height;

            events::window_resize_event event({width, height});
            data.event_func(event); });
    }

    void window::set_event_function(const std::function<void(events::base_event &)> func)
    {
        m_data.event_func = func;
    }
}
