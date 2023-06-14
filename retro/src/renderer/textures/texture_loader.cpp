#include "rtpch.h"
#include "texture_loader.h"

#include <stb_image.h>

namespace retro::renderer
{
    std::shared_ptr<texture> texture_loader::load_texture_from_file(const std::string &file_path)
    {
        RT_PROFILE_SECTION("texture_loader::load_texture_from_file");
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading texture from file.");
        const texture_data &data = parse_texture_file_contents(file_path);
        auto created_texture = std::make_shared<texture>(file_path, data);
        RT_TRACE("Retro Renderer | Texture loaded from file successfully!");
        RT_SEPARATOR();
        return created_texture;
    }

    std::shared_ptr<texture> texture_loader::load_texture_cubemap_from_file(const std::string &file_path)
    {
        RT_PROFILE_SECTION("texture_loader::load_texture_cubemap_from_file");
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading texture cubemap from file.");
        const texture_data &data = parse_texture_cubemap_file_contents(file_path);
        auto created_texture = std::make_shared<texture>(file_path, data);
        RT_TRACE("Retro Renderer | Texture cubemap loaded from file successfully!");
        RT_SEPARATOR();
        return created_texture;
    }

    std::shared_ptr<texture> texture_loader::load_texture_from_data(const texture_data &raw_data)
    {
        RT_PROFILE_SECTION("texture_loader::load_texture_from_data");
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading texture from data.");
        auto created_texture = std::make_shared<texture>("from_data", raw_data);
        RT_TRACE("Retro Renderer | Texture loaded from data successfully!");
        RT_SEPARATOR();
        return created_texture;
    }

    std::shared_ptr<texture> texture_loader::load_texture_from_memory(const char *data, int size)
    {
        RT_PROFILE_SECTION("texture_loader::load_texture_from_memory");
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading texture from memory.");
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char *loaded_data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(data), size, &width,
                                                           &height,
                                                           &channels, 0);
        RT_ASSERT_MSG(loaded_data != nullptr, "Could not read data from texture image file!");
        RT_ASSERT_MSG(channels > 0, "Invalid texture channels count!");

        texture_data texture_data = {width, height, channels, texture_type::normal, loaded_data};
        auto created_texture = std::make_shared<texture>("from_memory", texture_data);
        RT_TRACE("Retro Renderer | Texture loaded from memory successfully!");
        RT_SEPARATOR();
        return created_texture;
    }

    texture_data texture_loader::parse_texture_file_contents(const std::string &file_path)
    {
        RT_PROFILE_SECTION("texture_loader::parse_texture_file_contents");
        RT_TRACE("Retro Renderer | Started parsing texture file.");
        RT_TRACE("  - File Path: '{0}'", file_path);
        int width, height, channels;
        uint8_t *data;
        stbi_set_flip_vertically_on_load(1);
        data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
        RT_ASSERT_MSG(data != nullptr, "Could not read data from texture image file!");
        RT_ASSERT_MSG(channels > 0, "Invalid texture channels count!");

        RT_TRACE("Retro Renderer | Texture file parsed successfully.");
        return {width, height, channels, texture_type::normal, data};
    }

    texture_data texture_loader::parse_texture_cubemap_file_contents(const std::string &file_path)
    {
        RT_PROFILE_SECTION("texture_loader::parse_texture_cubemap_file_contents");
        RT_TRACE("Retro Renderer | Started parsing texture cubemap file.");
        RT_TRACE("  - File Path: '{0}'", file_path);
        int width, height, channels;

        stbi_set_flip_vertically_on_load(1);
        float *data = stbi_loadf(file_path.c_str(), &width, &height, &channels, 0);
        RT_ASSERT_MSG(data != nullptr, "Could not read data from texture cubemap image file!");
        RT_ASSERT_MSG(channels > 0, "Invalid texture channels count!");

        RT_TRACE("Retro Renderer | Texture cubemap file parsed successfully.");
        return {
            width, height, channels, texture_internal_format::rgb16f, texture_type::hdr,
            (float *)data};
    }
}
