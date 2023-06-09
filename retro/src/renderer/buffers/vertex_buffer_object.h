#pragma once

#include "renderer/buffers/vertex_buffer_layout_descriptor.h"

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
        vertex_buffer_object(vertex_buffer_object_target buffer_target, int count = 0);
        ~vertex_buffer_object();

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }
        int get_count() const { return m_count; }
        vertex_buffer_object_target get_target() const { return m_buffer_target; }
        const std::shared_ptr<vertex_buffer_layout_descriptor> &get_layout_descriptor() const { return m_layout_descriptor; }

        void bind();
        void un_bind();

        void set_attribute(uint32_t index, int size, uint32_t type, int stride, const void *data);
        void set_data(vertex_buffer_object_usage buffer_usage, int size, const void *data) const;

        void set_layout_descriptor(const std::shared_ptr<vertex_buffer_layout_descriptor> &layout_descriptor) { m_layout_descriptor = layout_descriptor; };

        static uint32_t get_buffer_target_to_opengl(vertex_buffer_object_target buffer_target);
        static uint32_t get_buffer_usage_to_opengl(vertex_buffer_object_usage buffer_usage);

    private:
        uint32_t m_handle_id;
        int m_count;
        vertex_buffer_object_target m_buffer_target;
        std::shared_ptr<vertex_buffer_layout_descriptor> m_layout_descriptor;
    };
}