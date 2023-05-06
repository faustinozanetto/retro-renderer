#pragma once

#include "renderer/buffers/vertex_buffer_object.h"

namespace retro::renderer
{
    class vertex_array_object
    {
    public:
        vertex_array_object();
        ~vertex_array_object();

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }
        const std::vector<std::shared_ptr<vertex_buffer_object>> &get_vertex_buffers() const { return m_vertex_buffers; }

        void bind();
        void un_bind();

        void add_vertex_buffer(const std::shared_ptr<vertex_buffer_object> &vertex_buffer);

    private:
        uint32_t m_handle_id;
        uint32_t m_vbo_index;
        std::vector<std::shared_ptr<vertex_buffer_object>> m_vertex_buffers;
    };
}