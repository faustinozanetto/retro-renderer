#include "rtpch.h"
#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

namespace retro::renderer
{
    texture_data::texture_data(int width, int height, int channels, texture_type type, void* data)
    {
        this->width = width;
        this->height = height;
        this->channels = channels;
        this->data = data;
        this->type = type;
        this->formats = texture::get_texture_formats_from_channel_count(channels);
    }

    texture::texture(const std::string& name, const texture_data& texture_data) : asset(
        assets::asset_type::texture, name)
    {
        m_data = texture_data;
        m_data.mip_mamp_levels = static_cast<int>(floor(log2((std::min)(m_data.width, m_data.height))));

        RT_TRACE("  - Width: {0}px", m_data.width);
        RT_TRACE("  - Height: {0}px", m_data.height);
        RT_TRACE("  - Channels: {0}", m_data.channels);
        RT_TRACE("  - Mipmap Levels: {0}", m_data.mip_mamp_levels);
        RT_TRACE("  - Format: '{0}'", get_texture_format_to_string(m_data.formats.format));
        RT_TRACE("  - Internal Format: '{0}'", get_texture_internal_format_to_string(m_data.formats.internal_format));

        if (m_data.type == texture_type::normal)
        {
            // Create OpenGL texture
            glCreateTextures(GL_TEXTURE_2D, 1, &m_handle_id);
            glBindTexture(GL_TEXTURE_2D, m_handle_id);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTextureStorage2D(m_handle_id, m_data.mip_mamp_levels, get_texture_format_to_opengl(m_data.formats.format), m_data.width,
                               m_data.height);

            // Filtering
            set_filtering(texture_filtering_type::filter_min, texture_filtering::linear);
            set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);

            // Wrapping
            set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::repeat);
            set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::repeat);
        }
        else if (m_data.type == texture_type::hdr)
        {
            // Create OpenGL texture
            glCreateTextures(GL_TEXTURE_2D, 1, &m_handle_id);
            glBindTexture(GL_TEXTURE_2D, m_handle_id);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTextureStorage2D(m_handle_id, m_data.mip_mamp_levels, get_texture_format_to_opengl(m_data.formats.format), m_data.width,
                               m_data.height);

            // Wrapping
            set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::clamp_to_edge);
            set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::clamp_to_edge);

            // Filtering
            set_filtering(texture_filtering_type::filter_min, texture_filtering::linear);
            set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);

            glTextureSubImage2D(m_handle_id, 0, 0, 0, m_data.width, m_data.height,
                                get_texture_internal_format_to_opengl(m_data.formats.internal_format), GL_FLOAT,
                                m_data.data);
            glGenerateTextureMipmap(m_handle_id);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
        else if (m_data.type == texture_type::cubemap)
        {
            // Create OpenGL texture
            glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_handle_id);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle_id);
            // Create all 6 faces.
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, get_texture_format_to_opengl(m_data.formats.format),
                             m_data.width, m_data.height, 0, get_texture_internal_format_to_opengl(m_data.formats.internal_format),
                             GL_FLOAT, m_data.data);
            }

            // Wrapping
            set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::clamp_to_edge);
            set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::clamp_to_edge);
            set_wrapping(texture_wrapping_type::wrap_r, texture_wrapping::clamp_to_edge);

            // Filtering
            set_filtering(texture_filtering_type::filter_min, texture_filtering::linear);
            set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);
        }

        // Allocating memory.
        if (m_data.type == texture_type::normal)
        {
            glTextureSubImage2D(m_handle_id, 0, 0, 0, m_data.width, m_data.height,
                                get_texture_internal_format_to_opengl(m_data.formats.internal_format), GL_UNSIGNED_BYTE,
                                m_data.data);
            glGenerateTextureMipmap(m_handle_id);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
        stbi_image_free(m_data.data);
    }

    std::string texture::get_texture_filtering_to_string(texture_filtering filtering)
    {
        switch (filtering)
        {
        case texture_filtering::none:
            return "none";
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
        case texture_wrapping::none:
            return "none";
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
        case texture_filtering::none:
            return -1;
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
        case texture_wrapping::none:
            return -1;
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

    std::string texture::get_texture_internal_format_to_string(texture_internal_format internal_format)
    {
        switch (internal_format)
        {
        case texture_internal_format::rg:
            return "rg";
        case texture_internal_format::rgb:
            return "rgb";
        case texture_internal_format::rgba:
            return "rgba";
        case texture_internal_format::bgr:
            return "bgr";
        case texture_internal_format::bgra:
            return "bgra";
        case texture_internal_format::red:
            return "red";
        case texture_internal_format::green:
            return "green";
        case texture_internal_format::blue:
            return "blue";
        case texture_internal_format::alpha:
            return "alpha";
        }
        RT_ASSERT_MSG(false, "Invalid texture internal format!");
    }

    std::string texture::get_texture_format_to_string(texture_format format)
    {
        switch (format)
        {
        case texture_format::r8:
            return "r8";
        case texture_format::r16:
            return "r16";
        case texture_format::r16f:
            return "r16f";
        case texture_format::r32f:
            return "r32f";
        case texture_format::rg8:
            return "rg8";
        case texture_format::rg16:
            return "rg16";
        case texture_format::rg16f:
            return "rg16f";
        case texture_format::rg32f:
            return "rg32f";
        case texture_format::rgb8:
            return "rgb8";
        case texture_format::rgb16:
            return "rgb16";
        case texture_format::rgb16f:
            return "rgb16f";
        case texture_format::rgb32f:
            return "rgb32f";
        case texture_format::rgba8:
            return "rgba8";
        case texture_format::rgba16:
            return "rgba16";
        case texture_format::rgba16f:
            return "rgba16f";
        case texture_format::rgba32f:
            return "rgba32f";
        case texture_format::depth_component16:
            return "depth_component16";
        case texture_format::depth_component24:
            return "depth_component24";
        case texture_format::depth_component32:
            return "depth_component32";
        case texture_format::depth_component32f:
            return "depth_component32f";
        case texture_format::stencil_index8:
            return "stencil_index8";
        }
        RT_ASSERT_MSG(false, "Invalid texture format!");
    }

    uint32_t texture::get_texture_internal_format_to_opengl(texture_internal_format internal_format)
    {
        switch (internal_format)
        {
        case texture_internal_format::rg:
            return GL_RG;
        case texture_internal_format::rgb:
            return GL_RGB;
        case texture_internal_format::rgba:
            return GL_RGBA;
        case texture_internal_format::bgr:
            return GL_BGR;
        case texture_internal_format::bgra:
            return GL_BGRA;
        case texture_internal_format::red:
            return GL_RED;
        case texture_internal_format::green:
            return GL_GREEN;
        case texture_internal_format::blue:
            return GL_BLUE;
        case texture_internal_format::alpha:
            return GL_ALPHA;
        }
        RT_ASSERT_MSG(false, "Invalid texture internal format!");
    }

    uint32_t texture::get_texture_format_to_opengl(texture_format format)
    {
        switch (format)
        {
        case texture_format::r8:
            return GL_R8;
        case texture_format::r16:
            return GL_R16;
        case texture_format::r16f:
            return GL_R16F;
        case texture_format::r32f:
            return GL_R32F;
        case texture_format::rg8:
            return GL_RG8;
        case texture_format::rg16:
            return GL_RG16;
        case texture_format::rg16f:
            return GL_RG16F;
        case texture_format::rg32f:
            return GL_RG32F;
        case texture_format::rgb8:
            return GL_RGB8;
        case texture_format::rgb16:
            return GL_RGB16;
        case texture_format::rgb16f:
            return GL_RGB16F;
        case texture_format::rgb32f:
            return GL_RGB32F;
        case texture_format::rgba8:
            return GL_RGBA8;
        case texture_format::rgba16:
            return GL_RGBA16;
        case texture_format::rgba16f:
            return GL_RGBA16F;
        case texture_format::rgba32f:
            return GL_RGBA32F;
        case texture_format::depth_component16:
            return GL_DEPTH_COMPONENT16;
        case texture_format::depth_component24:
            return GL_DEPTH_COMPONENT24;
        case texture_format::depth_component32:
            return GL_DEPTH_COMPONENT32;
        case texture_format::depth_component32f:
            return GL_DEPTH_COMPONENT32F;
        case texture_format::stencil_index8:
            return GL_STENCIL_INDEX8;
        }
        RT_ASSERT_MSG(false, "Invalid texture format!");
    }

    texture_formats texture::get_texture_formats_from_channel_count(int channel_count)
    {
        RT_ASSERT_MSG(channel_count > 0 && channel_count < 5, "Invalid texture chnnaels count!");
        texture_formats formats{};
        if (channel_count == 4)
        {
            formats.format = texture_format::rgba16f;
            formats.internal_format = texture_internal_format::rgba;
        }
        else if (channel_count == 3)
        {
            formats.format = texture_format::rgb16f;
            formats.internal_format = texture_internal_format::rgb;
        }
        else if (channel_count == 2)
        {
            formats.format = texture_format::rg16f;
            formats.internal_format = texture_internal_format::rg;
        }
        else if (channel_count == 1)
        {
            formats.format = texture_format::r16f;
            formats.internal_format = texture_internal_format::red;
        }
        return formats;
    }

    void texture::set_filtering(texture_filtering_type filtering_type, texture_filtering filtering)
    {
        glTextureParameteri(m_handle_id, get_texture_filtering_type_to_opengl(filtering_type),
                            get_texture_filtering_to_opengl(filtering));
    }

    void texture::set_wrapping(texture_wrapping_type wrapping_type, texture_wrapping wrapping)
    {
        glTextureParameteri(m_handle_id, get_texture_wrapping_type_to_opengl(wrapping_type),
                            get_texture_wrapping_to_opengl(wrapping));
    }

    void texture::serialize(std::ofstream& asset_pack_file)
    {
        // Serialize the texture's data size
        const size_t data_size = m_data.width * m_data.height * m_data.channels;
        asset_pack_file.write(reinterpret_cast<const char*>(&data_size), sizeof(data_size));
        
        // Serialize the texture's metadata (width, height, channels, mipmap levels)
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.width), sizeof(m_data.width));
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.height), sizeof(m_data.height));
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.channels), sizeof(m_data.channels));
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.mip_mamp_levels), sizeof(m_data.mip_mamp_levels));

        // Serialize the texture's metadata (texture type)
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.type), sizeof(m_data.type));

        // Serialize the texture's metadata (texture formats)
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.formats), sizeof(m_data.formats));

        // Serialize the texture's data
        asset_pack_file.write(static_cast<const char*>(m_data.data), data_size);
    }

    void texture::deserialize(std::ifstream& asset_pack_file)
    {
    }
}
