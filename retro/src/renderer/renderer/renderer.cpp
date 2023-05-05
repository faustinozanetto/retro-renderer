#include "rtpch.h"
#include "renderer.h"

#include "core/application.h"
#include "renderer/context/renderer_context.h"

namespace retro::renderer
{
    static renderer_data s_data;

    void renderer::initialize()
    {
        RT_TRACE("Retro Renderer | Renderer initialization started.");
        core::application &application = core::application::get();
        renderer_context::initialize(application.get_window());

        s_data.window_handle = application.get_window()->get_handle();
        RT_TRACE("Retro Renderer | Renderer initialization completed.");
    }

    bool renderer::get_window_should_close()
    {
        return glfwWindowShouldClose(s_data.window_handle);
    }

    void renderer::clear_screen()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void renderer::poll_input()
    {
        glfwPollEvents();
    }

    void renderer::swap_buffers()
    {
        glfwSwapBuffers(s_data.window_handle);
    }

    void renderer::set_clear_color(const glm::vec4 &clear_color)
    {
        s_data.clear_color = clear_color;
        glClearColor(s_data.clear_color.r, s_data.clear_color.g, s_data.clear_color.b, s_data.clear_color.a);
    }

    void renderer::set_vsync_enabled(bool is_vsync_enabled)
    {
        s_data.vsync_enabled = is_vsync_enabled;
        glfwSwapInterval(s_data.vsync_enabled ? 1 : 0);
    }
}
