#include "rtpch.h"
#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

namespace retro::renderer
{
    texture::texture(const std::string &file_path)
    {
        RT_TRACE("Retro Renderer | Started loading texture.");
        RT_TRACE("  - File Path: '{0}'", file_path);
        m_file_path = file_path;
        parse_texture_file();
        RT_TRACE("Retro Renderer | Texture loaded successfully.");
    }

    void texture::parse_texture_file()
    {
        int width, height, channels;
        stbi_uc *data;
        stbi_set_flip_vertically_on_load(1);
        {
            data = stbi_load(m_file_path.c_str(), &width, &height, &channels, 0);
        }
        assert(data);

        m_width = width;
        m_height = height;
        m_channels = channels;

        assert(m_channels > 0);

        if (m_channels == 4)
        {
            m_format = GL_RGBA8;
            m_data_format = GL_RGBA;
        }
        else if (m_channels == 3)
        {
            m_format = GL_RGB8;
            m_data_format = GL_RGB;
        }
        else if (m_channels == 2)
        {
            m_format = GL_RG8;
            m_data_format = GL_RG;
        }
        else if (m_channels == 1)
        {
            m_format = GL_R8;
            m_data_format = GL_RED;
        }

        RT_TRACE("  - Width: {0}", m_width);
        RT_TRACE("  - Height: {0}", m_height);
        RT_TRACE("  - Channels: {0}", m_channels);

        m_mipmap_levels = floor(log2((std::min)(m_width, m_height)));

        // Create OpenGL texture
        glCreateTextures(GL_TEXTURE_2D, 1, &m_handle_id);
        glBindTexture(GL_TEXTURE_2D, m_handle_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage2D(m_handle_id, m_mipmap_levels, m_format, m_width, m_height);

        // Filtering
        glTextureParameteri(m_handle_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_handle_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Wrapping
        glTextureParameteri(m_handle_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_handle_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Allocating memory.
        glTextureSubImage2D(m_handle_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_handle_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        stbi_image_free(data);
    }
}