#include "rtpch.h"
#include "vertex_array_object.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace retro::renderer
{
    vertex_array_object::vertex_array_object()
    {
        glGenVertexArrays(1, &m_handle_id);
    }

    vertex_array_object::~vertex_array_object()
    {
        glDeleteVertexArrays(1, &m_handle_id);
    }

    void vertex_array_object::bind()
    {
        glBindVertexArray(m_handle_id);
    }

    void vertex_array_object::un_bind()
    {
        glBindVertexArray(0);
    }
}