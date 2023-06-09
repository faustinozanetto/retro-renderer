#pragma once

#include "renderer/textures/texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace retro::renderer
{
    enum class render_buffer_attachment_type
    {
        color,
        depth,
        stencil,
        depth_stencil
    };

    class render_buffer
    {
    public:
        render_buffer(int width, int height, texture_internal_format internal_format);
        ~render_buffer();

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }
        int get_width() const { return m_width; }
        int get_height() const { return m_height; }
        texture_internal_format get_internal_format() const { return m_internal_format; }

        /* Setters */
        void set_storage_parameters(int width, int height, texture_internal_format internal_format);

        void bind();
        void un_bind();
        void attach_to_frame_buffer(render_buffer_attachment_type attachment_type);

        void resize(const glm::ivec2 &dimensions);

        static uint32_t get_render_buffer_attachment_type_to_opengl(render_buffer_attachment_type attachment_type);
        static std::string get_render_buffer_attachment_type_to_string(render_buffer_attachment_type attachment_type);

    private:
        uint32_t m_handle_id;
        int m_width;
        int m_height;
        texture_internal_format m_internal_format;
    };
}