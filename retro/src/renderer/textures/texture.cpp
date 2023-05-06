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

    std::string texture::get_texture_filtering_to_string(texture_filtering filtering)
    {
        switch (filtering)
        {
        case texture_filtering::nearest:
            return "nearest";
        case texture_filtering::linear:
            return "linear";
        case texture_filtering::nearest_mipmap_nearest:
            return "nearest_mipmap_nearest";
        case texture_filtering::linear_mipmap_nearest:
            return "linear_mipmap_nearest";
        case texture_filtering::nearest_mipmap_linear:
            return "nearest_mipmap_linear";
        case texture_filtering::linear_mipmap_linear:
            return "linear_mipmap_linear";
        }
        RT_ASSERT_MSG(false, "Invalid texture filtering!");
    }

    std::string texture::get_texture_filtering_type_to_string(texture_filtering_type filtering_type)
    {
        switch (filtering_type)
        {
        case texture_filtering_type::filter_min:
            return "filter_min";
        case texture_filtering_type::filter_mag:
            return "filter_mag";
        }
        RT_ASSERT_MSG(false, "Invalid texture filtering type!");
    }

    std::string texture::get_texture_wrapping_to_string(texture_wrapping wrapping)
    {
        switch (wrapping)
        {
        case texture_wrapping::repeat:
            return "repeat";
        case texture_wrapping::mirrored_repeat:
            return "mirrored_repeat";
        case texture_wrapping::clamp_to_edge:
            return "clamp_to_edge";
        case texture_wrapping::clamp_to_border:
            return "clamp_to_border";
        }
        RT_ASSERT_MSG(false, "Invalid texture wrapping!");
    }

    std::string texture::get_texture_wrapping_type_to_string(texture_wrapping_type wrapping_type)
    {
        switch (wrapping_type)
        {
        case texture_wrapping_type::wrap_r:
            return "wrap_r";
        case texture_wrapping_type::wrap_s:
            return "wrap_s";
        case texture_wrapping_type::wrap_t:
            return "wrap_t";
        }
        RT_ASSERT_MSG(false, "Invalid texture wrapping type!");
    }

    uint32_t texture::get_texture_filtering_to_opengl(texture_filtering filtering)
    {
        switch (filtering)
        {
        case texture_filtering::nearest:
            return GL_NEAREST;
        case texture_filtering::linear:
            return GL_LINEAR;
        case texture_filtering::nearest_mipmap_nearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        case texture_filtering::linear_mipmap_nearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        case texture_filtering::nearest_mipmap_linear:
            return GL_NEAREST_MIPMAP_LINEAR;
        case texture_filtering::linear_mipmap_linear:
            return GL_LINEAR_MIPMAP_LINEAR;
        }
        RT_ASSERT_MSG(false, "Invalid texture filtering!");
    }

    uint32_t texture::get_texture_filtering_type_to_opengl(texture_filtering_type filtering_type)
    {
        switch (filtering_type)
        {
        case texture_filtering_type::filter_min:
            return GL_TEXTURE_MIN_FILTER;
        case texture_filtering_type::filter_mag:
            return GL_TEXTURE_MAG_FILTER;
        }
        RT_ASSERT_MSG(false, "Invalid texture filtering type!");
    }

    uint32_t texture::get_texture_wrapping_to_opengl(texture_wrapping wrapping)
    {
        switch (wrapping)
        {
        case texture_wrapping::repeat:
            return GL_REPEAT;
        case texture_wrapping::mirrored_repeat:
            return GL_MIRRORED_REPEAT;
        case texture_wrapping::clamp_to_edge:
            return GL_CLAMP_TO_EDGE;
        case texture_wrapping::clamp_to_border:
            return GL_CLAMP_TO_BORDER;
        }
        RT_ASSERT_MSG(false, "Invalid texture wrapping!");
    }

    uint32_t texture::get_texture_wrapping_type_to_opengl(texture_wrapping_type wrapping_type)
    {
        switch (wrapping_type)
        {
        case texture_wrapping_type::wrap_r:
            return GL_TEXTURE_WRAP_R;
        case texture_wrapping_type::wrap_s:
            return GL_TEXTURE_WRAP_S;
        case texture_wrapping_type::wrap_t:
            return GL_TEXTURE_WRAP_T;
        }
        RT_ASSERT_MSG(false, "Invalid texture wrapping type!");
    }

    void texture::parse_texture_file()
    {
        int width, height, channels;
        stbi_uc *data;
        stbi_set_flip_vertically_on_load(1);
        {
            data = stbi_load(m_file_path.c_str(), &width, &height, &channels, 0);
        }
        RT_ASSERT_MSG(data != nullptr, "Could not read data from texture image!");

        m_width = width;
        m_height = height;
        m_channels = channels;
        m_mipmap_levels = floor(log2((std::min)(m_width, m_height)));

        RT_ASSERT_MSG(m_channels > 0, "Invalid texture channels count!");

        RT_TRACE("  - Width: {0}px", m_width);
        RT_TRACE("  - Height: {0}px", m_height);
        RT_TRACE("  - Channels: {0}", m_channels);
        RT_TRACE("  - Mip Maps: {0}", m_mipmap_levels);

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

        // Create OpenGL texture
        glCreateTextures(GL_TEXTURE_2D, 1, &m_handle_id);
        glBindTexture(GL_TEXTURE_2D, m_handle_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage2D(m_handle_id, m_mipmap_levels, m_format, m_width, m_height);

        // Filtering
        set_filtering(texture_filtering_type::filter_min, texture_filtering::linear);
        set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);

        // Wrapping
        set_wrapping(texture_wrapping_type::wrap_r, texture_wrapping::repeat);
        set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::repeat);
        set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::repeat);

        // Allocating memory.
        glTextureSubImage2D(m_handle_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_handle_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        stbi_image_free(data);
    }

    void texture::set_filtering(texture_filtering_type filtering_type, texture_filtering filtering)
    {
        glTextureParameteri(m_handle_id, get_texture_filtering_type_to_opengl(filtering_type), get_texture_filtering_to_opengl(filtering));
    }

    void texture::set_wrapping(texture_wrapping_type wrapping_type, texture_wrapping wrapping)
    {
        glTextureParameteri(m_handle_id, get_texture_wrapping_type_to_opengl(wrapping_type), get_texture_wrapping_to_opengl(wrapping));
    }
}