#pragma once

#include "renderer/window/window.h"
#include "renderer/buffers/vertex_array_object.h"
#include "renderer/models/model.h"
#include "renderer/text/font.h"
#include "renderer/text/text.h"
#include "renderer/shaders/shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace retro::renderer
{
    struct renderer_data
    {
        std::shared_ptr<window> window;
        glm::mat4 text_projection;
        glm::vec4 clear_color;
        bool vsync_enabled;
    };

    enum class renderer_draw_mode
    {
        points,
        line_strip,
        line_loop,
        lines,
        triangle_strip,
        triangle_fan,
        triangles
    };

    enum class renderer_state
    {
        depth,
        blend,
        face_cull
    };

    class renderer
    {
    public:
        static void initialize();

        /* Getters */
        static glm::ivec2 get_viewport_size();
        static bool get_is_vsync_enabled();
        static bool get_window_should_close();

        /* Functions */
        static void clear_screen();
        static void poll_input();
        static void swap_buffers();
        static void set_clear_color(const glm::vec4& clear_color);
        static void set_vsync_enabled(bool is_vsync_enabled);
        static void set_viewport_size(const glm::ivec2& viewport_size);
        static void set_state(renderer_state state, bool is_enabled);

        static void set_line_width(float line_width = 1.0f);

        static void bind_texture(uint32_t slot, uint32_t handle_id);

        static void submit_text(const std::shared_ptr<shader>& shader, const std::shared_ptr<font>& font,
                                const std::shared_ptr<text>& text);

        /* Primitive Rendering */
        static void submit_arrays(int count, renderer_draw_mode draw_mode = renderer_draw_mode::triangles);
        static void submit_arrays_instanced(int count, int instance_count,
                                            renderer_draw_mode draw_mode = renderer_draw_mode::triangles);
        static void submit_elements(int count, renderer_draw_mode draw_mode = renderer_draw_mode::triangles);

        /* VAO Rendering */
        static void submit_vao(const std::shared_ptr<vertex_array_object>& vao, int count,
                               renderer_draw_mode draw_mode = renderer_draw_mode::triangles);
        static void submit_vao_instanced(const std::shared_ptr<vertex_array_object>& vao, int count, int instance_count,
                                         renderer_draw_mode draw_mode = renderer_draw_mode::triangles);

        /* Mesh Rendering */
        static void submit_mesh(mesh* mesh, renderer_draw_mode draw_mode = renderer_draw_mode::triangles);
        static void submit_mesh(const std::shared_ptr<mesh>& mesh,
                                renderer_draw_mode draw_mode = renderer_draw_mode::triangles);

        /* Model Rendering */
        static void submit_model(const std::shared_ptr<model>& model,
                                 renderer_draw_mode draw_mode = renderer_draw_mode::triangles);
        static void submit_model_instanced(const std::shared_ptr<model>& model, int instance_count,
                                           renderer_draw_mode draw_mode = renderer_draw_mode::triangles);

        static void set_text_projection();

        /* Utils */
        static uint32_t get_renderer_state_to_opengl(renderer_state state);
        static uint32_t get_renderer_draw_mode_to_opengl(renderer_draw_mode draw_mode);
    };
}
