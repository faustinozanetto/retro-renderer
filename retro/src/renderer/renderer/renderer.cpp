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
        s_data.window = application.get_window();
        renderer_context::initialize(application.get_window());
        RT_TRACE("Retro Renderer | Renderer initialization completed.");
    }

    glm::ivec2 renderer::get_viewport_size()
    {
        return glm::ivec2(s_data.window->get_width(), s_data.window->get_height());
    }

    bool renderer::get_window_should_close()
    {
        return glfwWindowShouldClose(s_data.window->get_handle());
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
        glfwSwapBuffers(s_data.window->get_handle());
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

    void renderer::bind_texture(uint32_t slot, uint32_t handle_id)
    {
        glBindTextureUnit(slot, handle_id);
    }

    void renderer::submit_arrays(uint32_t draw_mode, int count)
    {
        glDrawArrays(draw_mode, 0, count);
    }

    void renderer::submit_vao(const std::shared_ptr<vertex_array_object> &vao, int count)
    {
        vao->bind();
        submit_arrays(GL_TRIANGLES, count);
        vao->un_bind();
    }

    void renderer::submit_vao_instanced(const std::shared_ptr<vertex_array_object> &vao, int count, int instance_count)
    {
        vao->bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, count, instance_count);
        vao->un_bind();
    }
}
