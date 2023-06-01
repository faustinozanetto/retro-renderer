#include "rtpch.h"
#include "renderer.h"

#include "core/application.h"
#include "renderer/context/renderer_context.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace retro::renderer
{
    static renderer_data s_data;

    void renderer::initialize()
    {
        RT_TRACE("Retro Renderer | Renderer initialization started.");
        core::application &application = core::application::get();
        s_data.window = application.get_window();
        set_text_projection();
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

    void renderer::set_viewport_size(const glm::ivec2 &viewport_size)
    {
        glViewport(0, 0, viewport_size.x, viewport_size.y);
        set_text_projection();
    }

    void renderer::set_depth_test(bool is_enabled)
    {
        if (is_enabled)
            glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }

    void renderer::bind_texture(uint32_t slot, uint32_t handle_id)
    {
        glBindTextureUnit(slot, handle_id);
    }

    void renderer::submit_text(const std::shared_ptr<shader> &shader, const std::shared_ptr<font> &font, const std::shared_ptr<text> &text)
    {
        shader->set_vec_float3("u_color", text->get_color());
        shader->set_mat4("u_projection", s_data.text_projection);

        uint32_t glyph_atlas = font->get_glyph_atlas();

        std::vector<text_vertex> text_vertices;

        float x = text->get_position().x;
        float y = text->get_position().y;
        for (char c : text->get_content())
        {
            // Retrieve the glyph information for the map
            const glyph_data& data = font->get_glyphs_data().at(c);

            float xpos = x + data.bearing.x * text->get_scale().x;
            float ypos = y - (data.size.y - data.bearing.y) * text->get_scale().y;

            float w = data.size.x * text->get_scale().x;
            float h = data.size.y * text->get_scale().y;

            text_vertex v1 = { glm::vec4(xpos,     ypos + h,   data.u1,data.v1) };
            text_vertex v2 = { glm::vec4(xpos,     ypos,       data.u1, data.v2) };
            text_vertex v3 = { glm::vec4(xpos + w, ypos,       data.u2, data.v2) };
            text_vertex v4 = { glm::vec4(xpos,     ypos + h,   data.u1, data.v1) };
            text_vertex v5 = { glm::vec4(xpos + w, ypos,       data.u2, data.v2) };
            text_vertex v6 = { glm::vec4(xpos + w, ypos + h,   data.u2, data.v1) };

            // Add the vertices to the vertices vector
            text_vertices.push_back(v1);
            text_vertices.push_back(v2);
            text_vertices.push_back(v3);
            text_vertices.push_back(v4);
            text_vertices.push_back(v5);
            text_vertices.push_back(v6);

            x += (data.advance >> 6) * text->get_scale().x; // Increment the x position based on the glyph advance
        }

        // Update vbo data
        auto &vbo = font->get_font_vao()->get_vertex_buffers()[0];
        vbo->bind();
        vbo->set_data(vertex_buffer_object_usage::dynamic_draw, sizeof(text_vertex) * text_vertices.size(), text_vertices.data());
        vbo->un_bind();

        font->get_font_vao()->bind();
        bind_texture(0, glyph_atlas);
        submit_arrays(GL_TRIANGLES, text_vertices.size());
        font->get_font_vao()->un_bind();
    }

    void renderer::submit_arrays(uint32_t draw_mode, int count)
    {
        glDrawArrays(draw_mode, 0, count);
    }

    void renderer::submit_elements(uint32_t draw_mode, int count)
    {
        glDrawElements(draw_mode, count, GL_UNSIGNED_INT, 0);
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

    void renderer::submit_model(const std::shared_ptr<model> &model)
    {
        for (const auto &mesh : model->get_meshes())
        {
            mesh->get_vao()->bind();
            submit_elements(GL_TRIANGLES, mesh->get_vao()->get_index_buffer()->get_count());
            mesh->get_vao()->un_bind();
        }
    }

    void renderer::set_text_projection()
    {
        s_data.text_projection = glm::ortho(0.0f, static_cast<float>(s_data.window->get_width()), 0.0f,
                                            static_cast<float>(s_data.window->get_height()));
    }
}
