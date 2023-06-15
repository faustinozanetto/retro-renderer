#include "rtpch.h"
#include "vertex_array_object.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace retro::renderer
{
    vertex_array_object::vertex_array_object()
    {
        m_vbo_index = 0;
        glGenVertexArrays(1, &m_handle_id);
    }

    vertex_array_object::~vertex_array_object()
    {
        glDeleteVertexArrays(1, &m_handle_id);
    }

    void vertex_array_object::bind()
    {
        RT_PROFILE;
        glBindVertexArray(m_handle_id);
    }

    void vertex_array_object::un_bind()
    {
        RT_PROFILE;
        glBindVertexArray(0);
    }

    void vertex_array_object::add_vertex_buffer(const std::shared_ptr<vertex_buffer_object> &vertex_buffer)
    {
        RT_PROFILE;
        RT_ASSERT_MSG(vertex_buffer->get_target() == vertex_buffer_object_target::arrays, "Vertex buffer must be of target 'arrays'!");

        glBindVertexArray(m_handle_id);
        vertex_buffer->bind();

        const std::shared_ptr<vertex_buffer_layout_descriptor> &layout_descriptor = vertex_buffer->get_layout_descriptor();

        for (const vertex_buffer_layout_entry &descriptor_element : layout_descriptor->get_layout_elements())
        {
            switch (descriptor_element.get_type())
            {
            case vertex_buffer_entry_type::mat3:
            case vertex_buffer_entry_type::mat4:
            {
                uint8_t count = vertex_buffer_layout_entry::get_vertex_buffer_entry_count(descriptor_element.get_type());
                for (uint8_t i = 0; i < count; i++)
                {
                    glEnableVertexAttribArray(m_vbo_index);
                    glVertexAttribPointer(m_vbo_index,
                                          count,
                                          vertex_buffer_layout_entry::get_vertex_buffer_entry_to_opengl(descriptor_element.get_type()),
                                          descriptor_element.get_is_normalized() ? GL_TRUE : GL_FALSE,
                                          layout_descriptor->get_stride(),
                                          reinterpret_cast<const void *>(descriptor_element.get_offset() + sizeof(float) * count * i));
                    glVertexAttribDivisor(m_vbo_index, 1);
                    m_vbo_index++;
                }
                break;
            }

            case vertex_buffer_entry_type::sfloat:
            case vertex_buffer_entry_type::vec_float2:
            case vertex_buffer_entry_type::vec_float3:
            case vertex_buffer_entry_type::vec_float4:
            {
                glEnableVertexAttribArray(m_vbo_index);
                glVertexAttribPointer(m_vbo_index,
                                      vertex_buffer_layout_entry::get_vertex_buffer_entry_count(descriptor_element.get_type()),
                                      vertex_buffer_layout_entry::get_vertex_buffer_entry_to_opengl(descriptor_element.get_type()),
                                      descriptor_element.get_is_normalized() ? GL_TRUE : GL_FALSE,
                                      layout_descriptor->get_stride(),
                                      reinterpret_cast<const void *>(descriptor_element.get_offset()));
                m_vbo_index++;
                break;
            }
            case vertex_buffer_entry_type::sint:
            case vertex_buffer_entry_type::vec_int2:
            case vertex_buffer_entry_type::vec_int3:
            case vertex_buffer_entry_type::vec_int4:
            {
                glEnableVertexAttribArray(m_vbo_index);
                glVertexAttribIPointer(m_vbo_index,
                                       vertex_buffer_layout_entry::get_vertex_buffer_entry_count(descriptor_element.get_type()),
                                       vertex_buffer_layout_entry::get_vertex_buffer_entry_to_opengl(descriptor_element.get_type()),
                                       layout_descriptor->get_stride(),
                                       reinterpret_cast<const void *>(descriptor_element.get_offset()));
                m_vbo_index++;
                break;
            }
            }
        }

        m_vertex_buffers.push_back(vertex_buffer);
    }

    void vertex_array_object::set_index_buffer(const std::shared_ptr<vertex_buffer_object> &index_buffer)
    {
        RT_PROFILE;
        RT_ASSERT_MSG(index_buffer->get_target() == vertex_buffer_object_target::elements, "Index buffer must be of target 'elements'!");
        glBindVertexArray(m_handle_id);
        index_buffer->bind();
        m_index_buffer = index_buffer;
    }
}