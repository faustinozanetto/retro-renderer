#include "rtpch.h"
#include "frame_buffer.h"

#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
    frame_buffer::frame_buffer(int width, int height)
    {
        RT_TRACE("Retro Renderer | Started creating frame buffer.");
        m_width = width;
        m_height = height;
        m_has_depth_attachment = false;

        m_attachments_data = {};
        m_depth_attachment_data = {};

        RT_TRACE("  - Width: {0}px", m_width);
        RT_TRACE("  - Height: {0}px", m_height);
        RT_TRACE("  - Attachments Count: {0}", m_attachments_data.size());
        RT_TRACE("  - Has Depth Attachment: '{0}'", m_has_depth_attachment ? "true" : "false");
        pre_initialize();
        RT_TRACE("Retro Renderer | Frame buffer created successfully.");
    }

    frame_buffer::frame_buffer(const std::vector<frame_buffer_attachment> &attachments, int width, int height,
                               frame_buffer_attachment depth_attachment)
    {
        RT_TRACE("Retro Renderer | Started creating frame buffer.");
        m_width = width;
        m_height = height;
        m_has_depth_attachment = true;

        m_attachments_data = attachments;
        m_depth_attachment_data = depth_attachment;

        RT_TRACE("  - Width: {0}px", m_width);
        RT_TRACE("  - Height: {0}px", m_height);
        RT_TRACE("  - Attachments Count: {0}", m_attachments_data.size());
        RT_TRACE("  - Has Depth Attachment: '{0}'", m_has_depth_attachment ? "true" : "false");

        pre_initialize();
        RT_TRACE("Retro Renderer | Frame buffer created successfully.");
    }

    frame_buffer::frame_buffer(const std::vector<frame_buffer_attachment> &attachments, int width, int height)
    {
        RT_TRACE("Retro Renderer | Started creating frame buffer.");
        m_width = width;
        m_height = height;
        m_has_depth_attachment = false;

        m_attachments_data = attachments;
        m_depth_attachment_data = {};

        RT_TRACE("  - Width: {0}px", m_width);
        RT_TRACE("  - Height: {0}px", m_height);
        RT_TRACE("  - Attachments Count: {0}", m_attachments_data.size());
        RT_TRACE("  - Has Depth Attachment: '{0}'", m_has_depth_attachment ? "true" : "false");

        pre_initialize();
    }

    frame_buffer::~frame_buffer()
    {
        glDeleteFramebuffers(1, &m_handle_id);
        glDeleteTextures(m_attachments.size(), m_attachments.data());
        if (m_has_depth_attachment)
            glDeleteTextures(1, &m_depth_attachment);
    }

    void frame_buffer::initialize()
    {
        RT_PROFILE_SECTION("frame_buffer::initialize");
        bind(false);
        // Draw buffers.
        if (!m_attachments.empty())
        {
            const GLenum buffers[8] = {
                GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7};
            glDrawBuffers(m_attachments.size(), buffers);
        }
        else if (m_attachments.empty() && m_has_depth_attachment)
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        // Error checking
        const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        RT_ASSERT_MSG(fboStatus == GL_FRAMEBUFFER_COMPLETE,
                      "An error occurred while creating frame buffer: " + std::to_string(fboStatus))

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_initialized = true;
        RT_TRACE("Retro Renderer | Frame buffer created successfully.");
    }

    void frame_buffer::pre_initialize()
    {
        RT_PROFILE_SECTION("frame_buffer::pre_initialize");
        m_initialized = false;
        // Delete old data
        if (m_handle_id)
        {
            glDeleteFramebuffers(1, &m_handle_id);
            glDeleteTextures(m_attachments.size(), m_attachments.data());
            m_attachments.clear();
            if (m_has_depth_attachment)
            {
                glDeleteTextures(1, &m_depth_attachment);
                m_depth_attachment = 0;
            }
        }

        // Create fbo
        glGenFramebuffers(1, &m_handle_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle_id);
        // Add color attachments
        if (!m_attachments_data.empty())
        {
            m_attachments.resize(m_attachments_data.size());
            for (int i = 0; i < m_attachments.size(); i++)
            {
                glGenTextures(1, &m_attachments[i]);
                attach_color_texture(m_attachments_data[i], m_attachments[i], i);
            }
        }

        // Add depth attachment
        if (m_has_depth_attachment)
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
            glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
            attach_depth_texture(m_depth_attachment_data, m_depth_attachment);
        }
    }

    void frame_buffer::bind(bool set_viewport_size)
    {
        RT_PROFILE_SECTION("frame_buffer::bind");
        if (set_viewport_size)
            renderer::set_viewport_size({m_width, m_height});
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle_id);
    }

    void frame_buffer::un_bind()
    {
        RT_PROFILE_SECTION("frame_buffer::un_bind");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void frame_buffer::attach_color_texture(frame_buffer_attachment attachment, uint32_t handle_id, int index) const
    {
        RT_PROFILE_SECTION("frame_buffer::attach_color_texture");
        glBindTexture(GL_TEXTURE_2D, handle_id);
        texture_format format = texture::get_texture_format_from_internal_format(attachment.internal_format);
        glTexImage2D(GL_TEXTURE_2D, 0, texture::get_texture_internal_format_to_opengl(attachment.internal_format), attachment.size.x, attachment.size.y, 0,
                     texture::get_texture_format_to_opengl(format), GL_FLOAT, nullptr);

        // Filtering
        if (attachment.filtering != texture_filtering::none)
        {
            glTextureParameteri(
                handle_id, texture::get_texture_filtering_type_to_opengl(texture_filtering_type::filter_min),
                texture::get_texture_filtering_to_opengl(attachment.filtering));
            glTextureParameteri(
                handle_id, texture::get_texture_filtering_type_to_opengl(texture_filtering_type::filter_mag),
                texture::get_texture_filtering_to_opengl(attachment.filtering));
        }

        // Wrapping
        if (attachment.wrapping != texture_wrapping::none)
        {
            glTextureParameteri(handle_id, texture::get_texture_wrapping_type_to_opengl(texture_wrapping_type::wrap_s),
                                texture::get_texture_wrapping_to_opengl(attachment.wrapping));
            glTextureParameteri(handle_id, texture::get_texture_wrapping_type_to_opengl(texture_wrapping_type::wrap_t),
                                texture::get_texture_wrapping_to_opengl(attachment.wrapping));
        }

        // Attach texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, handle_id, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void frame_buffer::attach_depth_texture(frame_buffer_attachment attachment, uint32_t handle_id)
    {
        RT_PROFILE_SECTION("frame_buffer::attach_depth_texture");
        glBindTexture(GL_TEXTURE_2D, handle_id);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, m_width, m_height);

        // Filtering
        if (attachment.filtering != texture_filtering::none)
        {
            glTextureParameteri(
                handle_id, texture::get_texture_filtering_type_to_opengl(texture_filtering_type::filter_min),
                texture::get_texture_filtering_to_opengl(attachment.filtering));
            glTextureParameteri(
                handle_id, texture::get_texture_filtering_type_to_opengl(texture_filtering_type::filter_mag),
                texture::get_texture_filtering_to_opengl(attachment.filtering));
        }

        // Wrapping
        if (attachment.wrapping != texture_wrapping::none)
        {
            glTextureParameteri(handle_id, texture::get_texture_wrapping_type_to_opengl(texture_wrapping_type::wrap_s),
                                texture::get_texture_wrapping_to_opengl(attachment.wrapping));
            glTextureParameteri(handle_id, texture::get_texture_wrapping_type_to_opengl(texture_wrapping_type::wrap_t),
                                texture::get_texture_wrapping_to_opengl(attachment.wrapping));
        }

        // Attach texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, handle_id, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void frame_buffer::resize(const glm::ivec2 &dimensions, bool update_attachments)
    {
        RT_PROFILE_SECTION("frame_buffer::resize");
        m_width = dimensions.x;
        m_height = dimensions.y;
        if (update_attachments)
        {
            for (auto &attachment : m_attachments_data)
            {
                attachment.size = dimensions;
            }
        }
        pre_initialize();
        initialize();
    }

    void frame_buffer::attach_texture(const std::shared_ptr<texture> &texture, uint32_t target,
                                      render_buffer_attachment_type attachment, uint32_t texture_target,
                                      int mipmaps_level)
    {
        RT_PROFILE_SECTION("frame_buffer::attach_texture");
        glFramebufferTexture2D(target, render_buffer::get_render_buffer_attachment_type_to_opengl(attachment),
                               texture_target, texture->get_handle_id(), mipmaps_level);
     
        m_attachments.push_back(texture->get_handle_id());
        frame_buffer_attachment attachment_data;
        attachment_data.size = {texture->get_data().width, texture->get_data().height};
        attachment_data.filtering = texture_filtering::linear;
        attachment_data.wrapping = texture_wrapping::clamp_to_edge;
        attachment_data.internal_format = texture->get_data().internal_format;
        m_attachments_data.push_back(attachment_data);
        
    }
}
