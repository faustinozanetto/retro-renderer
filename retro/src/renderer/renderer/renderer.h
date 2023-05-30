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

    class renderer
    {
    public:
        static void initialize();

        static glm::ivec2 get_viewport_size();

        static bool get_window_should_close();
        static void clear_screen();
        static void poll_input();
        static void swap_buffers();
        static void set_clear_color(const glm::vec4 &clear_color);
        static void set_vsync_enabled(bool is_vsync_enabled);
        static void set_viewport_size(const glm::ivec2 &viewport_size);

        static void bind_texture(uint32_t slot, uint32_t handle_id);

        static void submit_text(const std::shared_ptr<shader> &shader, const std::shared_ptr<font> &font, const std::shared_ptr<text> &text);

        static void submit_arrays(uint32_t draw_mode, int count);
        static void submit_elements(uint32_t draw_mode, int count);
        static void submit_vao(const std::shared_ptr<vertex_array_object> &vao, int count);
        static void submit_vao_instanced(const std::shared_ptr<vertex_array_object> &vao, int count, int instance_count);

        static void submit_model(const std::shared_ptr<model> &model);
        
        static void set_text_projection();
    };
}
