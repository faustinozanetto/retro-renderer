#include "rtpch.h"
#include "texture_loader.h"

#include <stb_image.h>

namespace retro::renderer
{
    std::shared_ptr<texture> texture_loader::load_texture_from_file(const std::string& file_path)
    {
        const texture_data& data = parse_texture_file_contents(file_path);
        return std::make_shared<texture>(file_path, data);
    }

    std::shared_ptr<texture> texture_loader::load_texture_cubemap_from_file(const std::string& file_path)
    {
        const texture_data& data = parse_texture_cubemap_file_contents(file_path);
        return std::make_shared<texture>(file_path, data);
    }

    std::shared_ptr<texture> texture_loader::load_texture_from_data(const texture_data& raw_data)
    {
        return std::make_shared<texture>("data_texture", raw_data);
    }

    std::shared_ptr<texture> texture_loader::load_texture_from_memory(const char* data, int size)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* loaded_data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data), size, &width,
                                                           &height,
                                                           &channels, 0);
        RT_ASSERT_MSG(loaded_data != nullptr, "Could not read data from texture image file!");
        RT_ASSERT_MSG(channels > 0, "Invalid texture channels count!");

        texture_data texture_data = {width, height, channels, texture_type::normal, loaded_data};
        return std::make_shared<texture>("", texture_data);
    }

    texture_data texture_loader::parse_texture_file_contents(const std::string& file_path)
    {
        RT_TRACE("Retro Renderer | Started parsing texture file.");
        RT_TRACE("  - File Path: '{0}'", file_path);
        int width, height, channels;
        uint8_t* data;
        stbi_set_flip_vertically_on_load(1);
        data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
        RT_ASSERT_MSG(data != nullptr, "Could not read data from texture image file!");
        RT_ASSERT_MSG(channels > 0, "Invalid texture channels count!");

        RT_TRACE("Retro Renderer | Texture file parsed successfully.");
        return {width, height, channels, texture_type::normal, data};
    }

    texture_data texture_loader::parse_texture_cubemap_file_contents(const std::string& file_path)
    {
        RT_TRACE("Retro Renderer | Started parsing texture cubemap file.");
        RT_TRACE("  - File Path: '{0}'", file_path);
        int width, height, channels;

        stbi_set_flip_vertically_on_load(1);
        float* data = stbi_loadf(file_path.c_str(), &width, &height, &channels, 0);
        RT_ASSERT_MSG(data != nullptr, "Could not read data from texture cubemap image file!");
        RT_ASSERT_MSG(channels > 0, "Invalid texture channels count!");

        RT_TRACE("Retro Renderer | Texture cubemap file parsed successfully.");
        return {
            width, height, channels, {texture_format::rgb16f, texture_internal_format::rgb}, texture_type::hdr,
            (float*)data
        };
    }
}
