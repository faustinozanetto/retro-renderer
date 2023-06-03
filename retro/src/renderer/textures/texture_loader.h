#pragma once

#include "renderer/textures/texture.h"

namespace retro::renderer
{
    class texture_loader
    {
    public:
        static std::shared_ptr<texture> load_texture_from_file(const std::string &file_path);
        static std::shared_ptr<texture> load_texture_cubemap_from_file(const std::string &file_path);
        static std::shared_ptr<texture> load_texture_from_data(const texture_data &raw_data);

    private:
        static texture_data parse_texture_file_contents(const std::string &file_path);
        static texture_data parse_texture_cubemap_file_contents(const std::string &file_path);
    };
}