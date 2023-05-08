#include "rtpch.h"
#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

namespace retro::renderer
{
    texture::texture(const raw_texture_data &raw_texture_data)
    {
        m_width = raw_texture_data.width;
        m_height = raw_texture_data.height;
        m_channels = raw_texture_data.channels;
        m_mipmap_levels = floor(log2((std::min)(m_width, m_height)));

        RT_TRACE("  - Width: {0}px", m_width);
        RT_TRACE("  - Height: {0}px", m_height);
        RT_TRACE("  - Channels: {0}", m_channels);
        RT_TRACE("  - Mipmap Levels: {0}", m_mipmap_levels);

        infer_formats_from_channel_count();

        // Create OpenGL texture
        glCreateTextures(GL_TEXTURE_2D, 1, &m_handle_id);
        glBindTexture(GL_TEXTURE_2D, m_handle_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage2D(m_handle_id, m_mipmap_levels, get_texture_format_to_opengl(m_format), m_width, m_height);

        // Filtering
        set_filtering(texture_filtering_type::filter_min, texture_filtering::linear_mipmap_linear);
        set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);

        // Wrapping
        set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::repeat);
        set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::repeat);

        // Allocating memory.
        glTextureSubImage2D(m_handle_id, 0, 0, 0, m_width, m_height, get_texture_internal_format_to_opengl(m_internal_format), GL_UNSIGNED_BYTE, raw_texture_data.data);
        glGenerateTextureMipmap(m_handle_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
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

    void texture::infer_formats_from_channel_count()
    {
        RT_ASSERT_MSG(m_channels > 0 && m_channels < 5, "Invalid texture chnnaels count!");
        if (m_channels == 4)
        {
            m_format = texture_format::rgba16f;
            m_internal_format = texture_internal_format::rgba;
        }
        else if (m_channels == 3)
        {
            m_format = texture_format::rgb16f;
            m_internal_format = texture_internal_format::rgb;
        }
        else if (m_channels == 2)
        {
            m_format = texture_format::rg16f;
            m_internal_format = texture_internal_format::rg;
        }
        else if (m_channels == 1)
        {
            m_format = texture_format::r16f;
            m_internal_format = texture_internal_format::red;
        }
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