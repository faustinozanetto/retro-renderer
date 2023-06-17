#include "rtpch.h"
#include "uniform_buffer_object.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace retro::renderer
{
    uniform_buffer_object::uniform_buffer_object()
    {
        glGenBuffers(1, &m_handle_id);
        m_data = nullptr;
        m_size = 0;
    }

    uniform_buffer_object::~uniform_buffer_object()
    {
        glDeleteBuffers(1, &m_handle_id);
    }

    void uniform_buffer_object::initialize(uint32_t size, const void* data)
    {
        RT_PROFILE;
		m_data = (uint8_t*)data;
		m_size = size;
		glBindBuffer(GL_UNIFORM_BUFFER, m_handle_id);
		glBufferData(GL_UNIFORM_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
    }

    void uniform_buffer_object::bind(uint32_t slot)
    {
        RT_PROFILE;
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_handle_id);
    }

    void uniform_buffer_object::un_bind()
    {
        RT_PROFILE;
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void uniform_buffer_object::set_data(uint32_t size, const void *data)
    {
        RT_PROFILE;
        m_data = (uint8_t *)data;
        m_size = size;

        glBindBuffer(GL_UNIFORM_BUFFER, m_handle_id);

        if (size != m_size)
        {
            GLvoid *temp_data = nullptr;
            temp_data = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
            m_size = size;

            memcpy(temp_data, m_data, m_size);
            glUnmapBuffer(GL_UNIFORM_BUFFER);
        }
        else
        {
            glBufferSubData(GL_UNIFORM_BUFFER, 0, m_size, m_data);
        }
    }
}