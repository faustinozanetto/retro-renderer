#include "rtpch.h"
#include "renderer.h"

#include "core/application.h"
#include "renderer/context/renderer_context.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "renderer/shaders/shader_file_watcher.h"

namespace retro::renderer
{
    static renderer_data s_data;

    void renderer::initialize()
    {
        RT_PROFILE;
        RT_TRACE("Retro Renderer | Renderer initialization started.");
        core::application& application = core::application::get();
        s_data.window = application.get_window();
        set_text_projection();
        /* Renderer context */
        renderer_context::initialize(application.get_window());
        /* Shader file watcher */
        shader_file_watcher::initialize();
        RT_TRACE("Retro Renderer | Renderer initialization completed.");
    }

    glm::ivec2 renderer::get_viewport_size()
    {
        RT_PROFILE;
        return glm::ivec2(s_data.window->get_width(), s_data.window->get_height());
    }

    bool renderer::get_is_vsync_enabled()
    {
        return s_data.vsync_enabled;
    }

    bool renderer::get_window_should_close()
    {
        RT_PROFILE;
        return glfwWindowShouldClose(s_data.window->get_handle());
    }

    void renderer::clear_screen()
    {
        RT_PROFILE;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void renderer::poll_input()
    {
        RT_PROFILE;
        glfwPollEvents();
    }

    void renderer::swap_buffers()
    {
        RT_PROFILE;
        glfwSwapBuffers(s_data.window->get_handle());
#ifdef TRACY_ENABLE
        TracyGpuCollect;
#endif
    }

    void renderer::set_clear_color(const glm::vec4& clear_color)
    {
        RT_PROFILE;
        s_data.clear_color = clear_color;
        glClearColor(s_data.clear_color.r, s_data.clear_color.g, s_data.clear_color.b, s_data.clear_color.a);
    }

    void renderer::set_vsync_enabled(bool is_vsync_enabled)
    {
        RT_PROFILE;
        s_data.vsync_enabled = is_vsync_enabled;
        glfwSwapInterval(s_data.vsync_enabled ? 1 : 0);
    }

    void renderer::set_viewport_size(const glm::ivec2& viewport_size)
    {
        RT_PROFILE;
        glViewport(0, 0, viewport_size.x, viewport_size.y);
        set_text_projection();
    }

    void renderer::set_state(renderer_state state, bool is_enabled)
    {
        RT_PROFILE;
        if (is_enabled)
        {
            glEnable(get_renderer_state_to_opengl(state));
            return;
        }
        glDisable(get_renderer_state_to_opengl(state));
    }

    void renderer::set_line_width(float line_width)
    {
        glLineWidth(line_width);
    }

    void renderer::bind_texture(uint32_t slot, uint32_t handle_id)
    {
        RT_PROFILE;
        glBindTextureUnit(slot, handle_id);
    }

    void renderer::submit_text(const std::shared_ptr<shader>& shader, const std::shared_ptr<font>& font,
                               const std::shared_ptr<text>& text)
    {
        RT_PROFILE;
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

            text_vertex v1 = {glm::vec4(xpos, ypos + h, data.u1, data.v1)};
            text_vertex v2 = {glm::vec4(xpos, ypos, data.u1, data.v2)};
            text_vertex v3 = {glm::vec4(xpos + w, ypos, data.u2, data.v2)};
            text_vertex v4 = {glm::vec4(xpos, ypos + h, data.u1, data.v1)};
            text_vertex v5 = {glm::vec4(xpos + w, ypos, data.u2, data.v2)};
            text_vertex v6 = {glm::vec4(xpos + w, ypos + h, data.u2, data.v1)};

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
        auto& vbo = font->get_font_vao()->get_vertex_buffers()[0];
        vbo->bind();
        vbo->set_data(vertex_buffer_object_usage::dynamic_draw, sizeof(text_vertex) * text_vertices.size(),
                      text_vertices.data());
        vbo->un_bind();

        font->get_font_vao()->bind();
        bind_texture(0, glyph_atlas);
        submit_arrays(text_vertices.size(), renderer_draw_mode::triangles);
        font->get_font_vao()->un_bind();
    }

    void renderer::submit_arrays(int count, renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        glDrawArrays(get_renderer_draw_mode_to_opengl(draw_mode), 0, count);
    }

    void renderer::submit_arrays_instanced(int count, int instance_count, renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        glDrawArraysInstanced(get_renderer_draw_mode_to_opengl(draw_mode), 0, count, instance_count);
    }

    void renderer::submit_elements(int count, renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        glDrawElements(get_renderer_draw_mode_to_opengl(draw_mode), count, GL_UNSIGNED_INT, 0);
    }

    void renderer::submit_vao(const std::shared_ptr<vertex_array_object>& vao, int count, renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        vao->bind();
        submit_arrays(count, draw_mode);
        vao->un_bind();
    }

    void renderer::submit_vao_instanced(const std::shared_ptr<vertex_array_object>& vao,
                                        int count, int instance_count, renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        vao->bind();
        submit_arrays_instanced(count, instance_count, draw_mode);
        vao->un_bind();
    }

    void renderer::submit_mesh(mesh* mesh, renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        mesh->get_vao()->bind();
        submit_elements(mesh->get_vao()->get_index_buffer()->get_count(), draw_mode);
        mesh->get_vao()->un_bind();
    }

    void renderer::submit_mesh(const std::shared_ptr<mesh>& mesh, renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        mesh->get_vao()->bind();
        submit_elements(mesh->get_vao()->get_index_buffer()->get_count(), draw_mode);
        mesh->get_vao()->un_bind();
    }

    void renderer::submit_model(const std::shared_ptr<model>& model, renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        for (const auto& mesh : model->get_meshes())
        {
            submit_mesh(mesh, draw_mode);
        }
    }

    void renderer::submit_model_instanced(const std::shared_ptr<model>& model,
                                          int instance_count, renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        for (const auto& mesh : model->get_meshes())
        {
            submit_vao_instanced(mesh->get_vao(), mesh->get_vao()->get_index_buffer()->get_count(),
                                 instance_count, draw_mode);
        }
    }

    void renderer::set_text_projection()
    {
        RT_PROFILE;
        s_data.text_projection = glm::ortho(0.0f, static_cast<float>(s_data.window->get_width()), 0.0f,
                                            static_cast<float>(s_data.window->get_height()));
    }

    uint32_t renderer::get_renderer_state_to_opengl(renderer_state state)
    {
        RT_PROFILE;
        switch (state)
        {
        case renderer_state::depth:
            return GL_DEPTH_TEST;
        case renderer_state::blend:
            return GL_BLEND;
        case renderer_state::face_cull:
            return GL_CULL_FACE;
        }
        RT_ASSERT_MSG(false, "Unknown renderer state!");
        return -1;
    }

    uint32_t renderer::get_renderer_draw_mode_to_opengl(renderer_draw_mode draw_mode)
    {
        RT_PROFILE;
        switch (draw_mode)
        {
        case renderer_draw_mode::line_loop:
            return GL_LINE_LOOP;
        case renderer_draw_mode::line_strip:
            return GL_LINE_STRIP;
        case renderer_draw_mode::lines:
            return GL_LINES;
        case renderer_draw_mode::points:
            return GL_POINTS;
        case renderer_draw_mode::triangle_fan:
            return GL_TRIANGLE_FAN;
        case renderer_draw_mode::triangle_strip:
            return GL_TRIANGLE_STRIP;
        case renderer_draw_mode::triangles:
            return GL_TRIANGLES;
        }
        RT_ASSERT_MSG(false, "Unknown renderer draw mode!");
        return -1;
    }
}
