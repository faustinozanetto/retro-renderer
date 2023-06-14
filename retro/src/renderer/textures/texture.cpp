#include "rtpch.h"
#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "texture_loader.h"

namespace retro::renderer
{
    texture_data::texture_data(int width, int height, int channels, texture_type type, void *data)
    {
        this->width = width;
        this->height = height;
        this->channels = channels;
        this->data = data;
        this->type = type;
        this->internal_format = texture::get_texture_formats_from_channel_count(channels).internal_format;
    }

    texture::texture(const std::string &file_name, const texture_data &texture_data) : asset(
                                                                                           {assets::asset_type::texture, file_name})
    {
        m_data = texture_data;
        if (m_data.mip_map_levels != 0)
            m_data.mip_map_levels = static_cast<int>(floor(log2((std::min)(m_data.width, m_data.height))));

        RT_TRACE("Texture Information:");
        RT_TRACE("  - Width: {0}px", m_data.width);
        RT_TRACE("  - Height: {0}px", m_data.height);
        RT_TRACE("  - Channels: {0}", m_data.channels);
        RT_TRACE("  - Mipmap Levels: {0}", m_data.mip_map_levels);
        RT_TRACE("  - Internal Format: '{0}'", get_texture_internal_format_to_string(m_data.internal_format));

        initialize();
    }

    texture::~texture()
    {
        glDeleteTextures(1, &m_handle_id);
    }

    texture_format texture::get_texture_format_from_internal_format(texture_internal_format internal_format)
    {
        RT_PROFILE_SECTION("texture::get_texture_format_from_internal_format");
        switch (internal_format)
        {
        case texture_internal_format::r8:
            return texture_format::red;
        case texture_internal_format::r16:
            return texture_format::red;
        case texture_internal_format::r16f:
            return texture_format::red;
        case texture_internal_format::r32f:
            return texture_format::red;
        case texture_internal_format::rg8:
            return texture_format::rg;
        case texture_internal_format::rg16:
            return texture_format::rg;
        case texture_internal_format::rg16f:
            return texture_format::rg;
        case texture_internal_format::rg32f:
            return texture_format::rg;
        case texture_internal_format::rgb8:
            return texture_format::rgb;
        case texture_internal_format::rgb16:
            return texture_format::rgb;
        case texture_internal_format::rgb16f:
            return texture_format::rgb;
        case texture_internal_format::rgb32f:
            return texture_format::rgb;
        case texture_internal_format::rgba8:
            return texture_format::rgba;
        case texture_internal_format::rgba16:
            return texture_format::rgba;
        case texture_internal_format::rgba16f:
            return texture_format::rgba;
        case texture_internal_format::rgba32f:
            return texture_format::rgba;
        case texture_internal_format::r11g11b10:
            return texture_format::rgb;
        case texture_internal_format::depth_component16:
            return texture_format::red;
        case texture_internal_format::depth_component24:
            return texture_format::red;
        case texture_internal_format::depth_component32:
            return texture_format::red;
        case texture_internal_format::depth_component32f:
            return texture_format::red;
        case texture_internal_format::stencil_index8:
            return texture_format::red;
        default:
            return texture_format::rgba;
        }
        RT_ASSERT_MSG(false, "Invalid texture internal format!");
    }

    std::string texture::get_texture_filtering_to_string(texture_filtering filtering)
    {
        RT_PROFILE_SECTION("texture::get_texture_filtering_to_string");
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
        RT_PROFILE_SECTION("texture::get_texture_filtering_type_to_string");
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
        RT_PROFILE_SECTION("texture::get_texture_wrapping_to_string");
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
        RT_PROFILE_SECTION("texture::get_texture_wrapping_type_to_string");
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

    int texture::get_texture_filtering_to_opengl(texture_filtering filtering)
    {
        RT_PROFILE_SECTION("texture::get_texture_filtering_to_opengl");
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

    int texture::get_texture_filtering_type_to_opengl(texture_filtering_type filtering_type)
    {
        RT_PROFILE_SECTION("texture::get_texture_filtering_type_to_opengl");
        switch (filtering_type)
        {
        case texture_filtering_type::filter_min:
            return GL_TEXTURE_MIN_FILTER;
        case texture_filtering_type::filter_mag:
            return GL_TEXTURE_MAG_FILTER;
        }
        RT_ASSERT_MSG(false, "Invalid texture filtering type!");
    }

    int texture::get_texture_wrapping_to_opengl(texture_wrapping wrapping)
    {
        RT_PROFILE_SECTION("texture::get_texture_wrapping_to_opengl");
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

    int texture::get_texture_wrapping_type_to_opengl(texture_wrapping_type wrapping_type)
    {
        RT_PROFILE_SECTION("texture::get_texture_wrapping_type_to_opengl");
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

    std::string texture::get_texture_format_to_string(texture_format format)
    {
        RT_PROFILE_SECTION("texture::get_texture_format_to_string");
        switch (format)
        {
        case texture_format::rg:
            return "rg";
        case texture_format::rgb:
            return "rgb";
        case texture_format::rgba:
            return "rgba";
        case texture_format::bgr:
            return "bgr";
        case texture_format::bgra:
            return "bgra";
        case texture_format::red:
            return "red";
        case texture_format::green:
            return "green";
        case texture_format::blue:
            return "blue";
        case texture_format::alpha:
            return "alpha";
        }
        RT_ASSERT_MSG(false, "Invalid texture format!");
    }

    std::string texture::get_texture_internal_format_to_string(texture_internal_format internal_format)
    {
        RT_PROFILE_SECTION("texture::get_texture_internal_format_to_string");
        switch (internal_format)
        {
        case texture_internal_format::r8:
            return "r8";
        case texture_internal_format::r16:
            return "r16";
        case texture_internal_format::r16f:
            return "r16f";
        case texture_internal_format::r32f:
            return "r32f";
        case texture_internal_format::rg8:
            return "rg8";
        case texture_internal_format::rg16:
            return "rg16";
        case texture_internal_format::rg16f:
            return "rg16f";
        case texture_internal_format::rg32f:
            return "rg32f";
        case texture_internal_format::rgb8:
            return "rgb8";
        case texture_internal_format::rgb16:
            return "rgb16";
        case texture_internal_format::rgb16f:
            return "rgb16f";
        case texture_internal_format::rgb32f:
            return "rgb32f";
        case texture_internal_format::rgba8:
            return "rgba8";
        case texture_internal_format::rgba16:
            return "rgba16";
        case texture_internal_format::rgba16f:
            return "rgba16f";
        case texture_internal_format::rgba32f:
            return "rgba32f";
        case texture_internal_format::r11g11b10:
            return "r11g11b10";
        case texture_internal_format::depth_component16:
            return "depth_component16";
        case texture_internal_format::depth_component24:
            return "depth_component24";
        case texture_internal_format::depth_component32:
            return "depth_component32";
        case texture_internal_format::depth_component32f:
            return "depth_component32f";
        case texture_internal_format::stencil_index8:
            return "stencil_index8";
        }
        RT_ASSERT_MSG(false, "Invalid texture internal format!");
    }

    int texture::get_texture_format_to_opengl(texture_format format)
    {
        RT_PROFILE_SECTION("texture::get_texture_format_to_opengl");
        switch (format)
        {
        case texture_format::rg:
            return GL_RG;
        case texture_format::rgb:
            return GL_RGB;
        case texture_format::rgba:
            return GL_RGBA;
        case texture_format::bgr:
            return GL_BGR;
        case texture_format::bgra:
            return GL_BGRA;
        case texture_format::red:
            return GL_RED;
        case texture_format::green:
            return GL_GREEN;
        case texture_format::blue:
            return GL_BLUE;
        case texture_format::alpha:
            return GL_ALPHA;
        }
        RT_ASSERT_MSG(false, "Invalid texture format!");
    }

    int texture::get_texture_internal_format_to_opengl(texture_internal_format internal_format)
    {
        RT_PROFILE_SECTION("texture::get_texture_internal_format_to_opengl");
        switch (internal_format)
        {
        case texture_internal_format::r8:
            return GL_R8;
        case texture_internal_format::r16:
            return GL_R16;
        case texture_internal_format::r16f:
            return GL_R16F;
        case texture_internal_format::r32f:
            return GL_R32F;
        case texture_internal_format::rg8:
            return GL_RG8;
        case texture_internal_format::rg16:
            return GL_RG16;
        case texture_internal_format::rg16f:
            return GL_RG16F;
        case texture_internal_format::rg32f:
            return GL_RG32F;
        case texture_internal_format::rgb8:
            return GL_RGB8;
        case texture_internal_format::rgb16:
            return GL_RGB16;
        case texture_internal_format::rgb16f:
            return GL_RGB16F;
        case texture_internal_format::rgb32f:
            return GL_RGB32F;
        case texture_internal_format::rgba8:
            return GL_RGBA8;
        case texture_internal_format::rgba16:
            return GL_RGBA16;
        case texture_internal_format::rgba16f:
            return GL_RGBA16F;
        case texture_internal_format::rgba32f:
            return GL_RGBA32F;
        case texture_internal_format::r11g11b10:
            return GL_R11F_G11F_B10F;
        case texture_internal_format::depth_component16:
            return GL_DEPTH_COMPONENT16;
        case texture_internal_format::depth_component24:
            return GL_DEPTH_COMPONENT24;
        case texture_internal_format::depth_component32:
            return GL_DEPTH_COMPONENT32;
        case texture_internal_format::depth_component32f:
            return GL_DEPTH_COMPONENT32F;
        case texture_internal_format::stencil_index8:
            return GL_STENCIL_INDEX8;
        }
        RT_ASSERT_MSG(false, "Invalid texture internal format!");
    }

    texture_formats texture::get_texture_formats_from_channel_count(int channel_count)
    {
        RT_PROFILE_SECTION("texture::get_texture_formats_from_channel_count");
        RT_ASSERT_MSG(channel_count > 0 && channel_count < 5, "Invalid texture chnnaels count!");
        texture_formats formats;
        if (channel_count == 4)
        {
            formats.internal_format = texture_internal_format::rgba16f;
            formats.format = texture_format::rgba;
        }
        else if (channel_count == 3)
        {
            formats.internal_format = texture_internal_format::rgb16f;
            formats.format = texture_format::rgb;
        }
        else if (channel_count == 2)
        {
            formats.internal_format = texture_internal_format::rg16f;
            formats.format = texture_format::rg;
        }
        else if (channel_count == 1)
        {
            formats.internal_format = texture_internal_format::r16f;
            formats.format = texture_format::red;
        }
        return formats;
    }

    void texture::initialize()
    {
        RT_PROFILE_SECTION("texture::initialize");
        if (m_data.type == texture_type::normal || m_data.type == texture_type::none)
        {
            // Create OpenGL texture
            glCreateTextures(GL_TEXTURE_2D, 1, &m_handle_id);
            glBindTexture(GL_TEXTURE_2D, m_handle_id);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTextureStorage2D(m_handle_id, 1, get_texture_internal_format_to_opengl(m_data.internal_format), m_data.width,
                               m_data.height);

            // Filtering
            set_filtering(texture_filtering_type::filter_min, texture_filtering::linear);
            set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);

            // Wrapping
            set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::clamp_to_edge);
            set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::clamp_to_edge);
        }
        else if (m_data.type == texture_type::hdr)
        {
            // Create OpenGL texture
            glCreateTextures(GL_TEXTURE_2D, 1, &m_handle_id);
            glBindTexture(GL_TEXTURE_2D, m_handle_id);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTextureStorage2D(m_handle_id, 1, get_texture_internal_format_to_opengl(m_data.internal_format), m_data.width,
                m_data.height);

            // Wrapping
            set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::clamp_to_edge);
            set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::clamp_to_edge);

            // Filtering
            set_filtering(texture_filtering_type::filter_min, texture_filtering::linear);
            set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);

            texture_format format = get_texture_format_from_internal_format(m_data.internal_format);
            glTextureSubImage2D(m_handle_id, 0, 0, 0, m_data.width, m_data.height,
                get_texture_format_to_opengl(format), GL_FLOAT,
                                m_data.data);
            glGenerateTextureMipmap(m_handle_id);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
        else if (m_data.type == texture_type::cubemap)
        {
            // Create OpenGL texture
            glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_handle_id);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle_id);
            texture_format format = get_texture_format_from_internal_format(m_data.internal_format);
            // Create all 6 faces.
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, get_texture_internal_format_to_opengl(m_data.internal_format),
                             m_data.width, m_data.height, 0,
                             get_texture_format_to_opengl(format),
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
            texture_format format = get_texture_format_from_internal_format(m_data.internal_format);
            glTextureSubImage2D(m_handle_id, 0, 0, 0, m_data.width, m_data.height, get_texture_format_to_opengl(format),
                                 m_data.internal_format == texture_internal_format::r11g11b10 ? GL_FLOAT
                                                                                                                                                                     : GL_UNSIGNED_BYTE,
                                m_data.data);

            /*
            glTextureSubImage2D(m_handle_id, 0, 0, 0, m_data.width, m_data.height,
                                get_texture_internal_format_to_opengl(m_data.formats.internal_format), GL_UNSIGNED_BYTE,
                                m_data.data);

            glTextureSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_data.width, m_data.height, get_texture_format_to_opengl(m_data.formats.format),
                                GL_UNSIGNED_BYTE, m_data.data);
*/
            // if (m_data.data != 0)
            // glGenerateTextureMipmap(m_handle_id);

            // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
    }

    void texture::set_filtering(texture_filtering_type filtering_type, texture_filtering filtering)
    {
        RT_PROFILE_SECTION("texture::set_filtering");
        glTextureParameteri(m_handle_id, get_texture_filtering_type_to_opengl(filtering_type),
                            get_texture_filtering_to_opengl(filtering));
    }

    void texture::set_wrapping(texture_wrapping_type wrapping_type, texture_wrapping wrapping)
    {
        RT_PROFILE_SECTION("texture::set_wrapping");
        glTextureParameteri(m_handle_id, get_texture_wrapping_type_to_opengl(wrapping_type),
                            get_texture_wrapping_to_opengl(wrapping));
    }

    void texture::serialize(std::ofstream &asset_pack_file)
    {
        RT_PROFILE_SECTION("texture::serialize");
        // Read texture file.
        std::ifstream texture_file(m_metadata.file_path, std::ios::binary | std::ios::ate);
        RT_ASSERT_MSG(texture_file.is_open(), "Failed to open sound file while serializing asset!");

        const std::streamsize size = texture_file.tellg();
        texture_file.seekg(0, std::ios::beg);

        // Read the file data into a temporary buffer
        std::vector<char> buffer(size);
        if (!texture_file.read(buffer.data(), size))
        {
            std::cerr << "Failed to read file: " << m_metadata.file_path << std::endl;
            return;
        }

        // Write the texture file size to the asset pack file
        asset_pack_file.write(reinterpret_cast<const char *>(&size), sizeof(std::streamsize));

        // Write the texture file data to the asset pack file
        asset_pack_file.write(buffer.data(), size);
    }

    std::shared_ptr<texture> texture::deserialize(const assets::asset_metadata &metadata,
                                                  std::ifstream &asset_pack_file)
    {
        RT_PROFILE_SECTION("texture::deserialize");
        // Read the texture file size from the asset pack file
        size_t data_size;
        asset_pack_file.read(reinterpret_cast<char *>(&data_size), sizeof(data_size));

        // Allocate memory for the texture data
        std::vector<char> data(data_size);

        // Deserialize the texture's data
        asset_pack_file.read(data.data(), data_size);

        const std::shared_ptr<texture> &texture = texture_loader::load_texture_from_memory(data.data(), data_size);
        texture->set_metadata(metadata);
        return texture;
    }
}
