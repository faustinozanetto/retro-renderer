#pragma once

#include "renderer/textures/texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace retro::renderer
{
    struct frame_buffer_attachment
    {
        uint32_t format;
        uint32_t data_format;
        texture_filtering filtering;
        texture_wrapping wrapping;
    };

    class frame_buffer
    {
    public:
        frame_buffer(const std::vector<frame_buffer_attachment> &attachments, int width, int height, bool has_depth_attachment, frame_buffer_attachment depth_attachment = {});
        ~frame_buffer();

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }
        uint32_t get_attachment_id(int index = 0) const { return m_attachments[index]; }

        void bind();
        void un_bind();

        void resize(const glm::ivec2 &dimensions);

    private:
        void initialize();
        void attach_color_texture(frame_buffer_attachment attachment, uint32_t handle_id, int index);
        void attach_depth_texture(frame_buffer_attachment attachment, uint32_t handle_id);

        uint32_t m_handle_id;
        int m_width;
        int m_height;
        bool m_has_depth_attachment;
        uint32_t m_depth_attachment;
        std::vector<uint32_t> m_attachments;
        frame_buffer_attachment m_depth_attachment_data;
        std::vector<frame_buffer_attachment> m_attachments_data;
    };
}