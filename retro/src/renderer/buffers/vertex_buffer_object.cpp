#include "rtpch.h"
#include "vertex_buffer_object.h"

namespace retro::renderer
{
    vertex_buffer_object::vertex_buffer_object(vertex_buffer_object_target buffer_target)
    {
        m_buffer_target = buffer_target;
        glGenBuffers(1, &m_handle_id);
    }

    vertex_buffer_object::~vertex_buffer_object()
    {
        glDeleteBuffers(1, &m_handle_id);
    }

    void vertex_buffer_object::bind()
    {
        glBindBuffer(get_buffer_target_to_opengl(m_buffer_target), m_handle_id);
    }

    void vertex_buffer_object::un_bind()
    {
        glBindBuffer(get_buffer_target_to_opengl(m_buffer_target), 0);
    }

    void vertex_buffer_object::set_data(
        vertex_buffer_object_usage buffer_usage, int size,
        const void *data) const
    {
        glBufferData(get_buffer_target_to_opengl(m_buffer_target), size, data, get_buffer_usage_to_opengl(buffer_usage));
    }

    uint32_t vertex_buffer_object::get_buffer_target_to_opengl(vertex_buffer_object_target buffer_target)
    {
        switch (buffer_target)
        {
        case vertex_buffer_object_target::arrays:
            return GL_ARRAY_BUFFER;
        case vertex_buffer_object_target::elements:
            return GL_ELEMENT_ARRAY_BUFFER;
        }
        assert(false);
        return -1;
    }

    static uint32_t get_buffer_usage_to_opengl(vertex_buffer_object_usage buffer_usage)
    {
        switch (buffer_usage)
        {
        case (vertex_buffer_object_usage::static_draw):
            return GL_STATIC_DRAW;
        case (vertex_buffer_object_usage::static_read):
            return GL_STATIC_READ;
        case (vertex_buffer_object_usage::static_copy):
            return GL_STATIC_COPY;
        case (vertex_buffer_object_usage::dynamic_draw):
            return GL_DYNAMIC_DRAW;
        case (vertex_buffer_object_usage::dynamic_read):
            return GL_DYNAMIC_READ;
        case (vertex_buffer_object_usage::dynamic_copy):
            return GL_DYNAMIC_COPY;
        }
        assert(false);
        return -1;
    }

}