#include "rtpch.h"
#include "render_buffer.h"

namespace retro::renderer
{
    render_buffer::render_buffer(int width, int height, texture_format format)
    {
        RT_TRACE("Retro Renderer | Started creating render buffer.");
        m_width = width;
        m_height = height;
        m_format = format;

        RT_TRACE("  - Width: {0}px", m_width);
        RT_TRACE("  - Height: {0}px", m_height);
        RT_TRACE("  - Format: '{0}'", texture::get_texture_format_to_string(format));

        glGenRenderbuffers(1, &m_handle_id);
        glBindRenderbuffer(GL_RENDERBUFFER, m_handle_id);
        glRenderbufferStorage(GL_RENDERBUFFER, texture::get_texture_format_to_opengl(m_format), m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        RT_TRACE("Retro Renderer | Frame buffer created successfully.");
    }

    render_buffer::~render_buffer()
    {
        glDeleteRenderbuffers(1, &m_handle_id);
    }

    void render_buffer::set_storage_parameters(int width, int height, texture_format format)
    {
        m_width = width;
        m_height = height;
        m_format = format;
        glBindRenderbuffer(GL_RENDERBUFFER, m_handle_id);
        glRenderbufferStorage(GL_RENDERBUFFER, texture::get_texture_format_to_opengl(m_format), m_width, m_height);
    }

    void render_buffer::bind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_handle_id);
    }

    void render_buffer::un_bind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void render_buffer::attach_to_frame_buffer(render_buffer_attachment_type attachment_type)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, get_render_buffer_attachment_type_to_opengl(attachment_type), GL_RENDERBUFFER, m_handle_id);
    }

    void render_buffer::resize(const glm::ivec2& dimensions)
    {
    }

    uint32_t render_buffer::get_render_buffer_attachment_type_to_opengl(render_buffer_attachment_type attachment_type)
    {
        switch (attachment_type)
        {
        case render_buffer_attachment_type::color:
            return GL_COLOR_ATTACHMENT0;
        case render_buffer_attachment_type::depth:
            return GL_DEPTH_ATTACHMENT;
        case render_buffer_attachment_type::stencil:
            return GL_STENCIL_ATTACHMENT;
        case render_buffer_attachment_type::depth_stencil:
            return GL_DEPTH_STENCIL_ATTACHMENT;
        }
        RT_ASSERT_MSG(false, "Invalid render buffer attachment type!");
    }

    std::string render_buffer::get_render_buffer_attachment_type_to_string(render_buffer_attachment_type attachment_type)
    {
        switch (attachment_type)
        {
        case render_buffer_attachment_type::color:
            return "color";
        case render_buffer_attachment_type::depth:
            return "depth";
        case render_buffer_attachment_type::stencil:
            return "stencil";
        case render_buffer_attachment_type::depth_stencil:
            return "depth_stencil";
        }
        RT_ASSERT_MSG(false, "Invalid render buffer attachment type!");
    }
}