#pragma once

#include "renderer/materials/material.h"

namespace retro::renderer
{
    class material_loader
    {
    public:
        static std::shared_ptr<material> load_material_default();
        static std::shared_ptr<material> load_material_from_data(const material_data& data);
        static std::shared_ptr<material> load_material_from_file(const std::string &file_path);

        static material_data parse_material_data_from_file(const std::string &file_path);
        static void parse_material_parameters_from_file(const std::string& line, material_data& material_data);
        static void parse_material_textures_from_file(const std::string& line, std::ifstream& material_file, material_data& material_data);
    };
}