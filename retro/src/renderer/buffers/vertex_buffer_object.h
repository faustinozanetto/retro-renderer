#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace retro::renderer
{
    enum class vertex_buffer_object_target
    {
        arrays,
        elements
    };

    enum class vertex_buffer_object_usage
    {
        static_draw,
        static_read,
        static_copy,
        dynamic_draw,
        dynamic_read,
        dynamic_copy
    };

    class vertex_buffer_object
    {
    public:
        vertex_buffer_object(vertex_buffer_object_target buffer_target);
        ~vertex_buffer_object();

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }

        void bind();
        void un_bind();

        void set_data(vertex_buffer_object_usage buffer_usage, int size, const void *data) const;

        static uint32_t get_buffer_target_to_opengl(vertex_buffer_object_target buffer_target);
        static uint32_t get_buffer_usage_to_opengl(vertex_buffer_object_usage buffer_usage);

    private:
        uint32_t m_handle_id;
        vertex_buffer_object_target m_buffer_target;
    };
}