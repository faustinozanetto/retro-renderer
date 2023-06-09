#include "rtpch.h"
#include "vertex_buffer_object.h"

namespace retro::renderer
{
    vertex_buffer_object::vertex_buffer_object(vertex_buffer_object_target buffer_target, int count)
    {
        m_buffer_target = buffer_target;
        m_count = count;
        // Create buffer
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
        glBindBuffer(get_buffer_target_to_opengl(m_buffer_target), m_handle_id);
        glBufferData(get_buffer_target_to_opengl(m_buffer_target), size, data, get_buffer_usage_to_opengl(buffer_usage));
    }

    void vertex_buffer_object::set_attribute(uint32_t index, int size, uint32_t type, int stride, const void *data)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, data);
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
        RT_ASSERT_MSG(false, "Invalid vertex buffer object target!");
        return -1;
    }

    uint32_t vertex_buffer_object::get_buffer_usage_to_opengl(vertex_buffer_object_usage buffer_usage)
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
        RT_ASSERT_MSG(false, "Invalid vertex buffer object usage!");
        return -1;
    }

}