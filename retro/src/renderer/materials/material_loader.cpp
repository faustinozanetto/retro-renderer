#include "rtpch.h"
#include "material_loader.h"

#include "assets/asset_manager.h"
#include "renderer/textures/texture.h"
#include "renderer/textures/texture_loader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace retro::renderer
{
    std::shared_ptr<material> material_loader::load_material_from_file(const std::string &file_path)
    {
        RT_PROFILE_SECTION("material_loader::load_material_from_file");
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading material from file.");
        const material_data material_data = parse_material_data_from_file(file_path);
        std::map<retro::renderer::material_texture_type, int> material_bindings;
        material_bindings[retro::renderer::material_texture_type::albedo] = 0;
        material_bindings[retro::renderer::material_texture_type::normal] = 1;
        material_bindings[retro::renderer::material_texture_type::roughness] = 2;
        material_bindings[retro::renderer::material_texture_type::metallic] = 3;
        material_bindings[retro::renderer::material_texture_type::ambient_occlusion] = 4;
        material_bindings[retro::renderer::material_texture_type::emissive] = 5;
        material_bindings[retro::renderer::material_texture_type::opacity] = 6;
        auto created_material = std::make_shared<material>(file_path, material_data, material_bindings);
        RT_TRACE("Retro Renderer | Material loaded from file successfully!");
        RT_SEPARATOR();
        return created_material;
    }

    material_data material_loader::parse_material_data_from_file(const std::string &file_path)
    {
        RT_PROFILE_SECTION("material_loader::parse_material_data_from_file");
        std::string shader_contents;
        std::ifstream material_file(file_path, std::ios::in | std::ios::binary);

        if (!material_file.is_open())
        {
            RT_ERROR("Retro Renderer | An error occurred while opening material file '{}'!", file_path);
            RT_ASSERT_MSG(false, "Retro Renderer | Could not open material file!");
        }

        material_data material_data;

        std::string line;
        std::string current_section;
        while (std::getline(material_file, line))
        {
            // Ignore empty lines and comments
            if (line.empty() || line[0] == '#')
            {
                continue;
            }
            else if (line[0] == '[' && line[line.length() - 2] == ']')
            {
                current_section = line.substr(1, line.length() - 3);
            }
            // Read each section
            else
            {
                if (current_section == "parameters")
                {
                    parse_material_parameters_from_file(line, material_data);
                }
                else if (current_section == "textures")
                {
                    parse_material_textures_from_file(line, material_file, material_data);
                }
            }
        }
        return material_data;
    }

    void material_loader::parse_material_parameters_from_file(const std::string &line, material_data &material_data)
    {
        RT_PROFILE_SECTION("material_loader::parse_material_parameters_from_file");
        std::istringstream iss(line);
        std::string param;
        if (iss >> param)
        {
            // Remove : from param
            param = param.substr(0, param.length() - 1);
            if (param == "albedo")
            {
                iss >> material_data.albedo.r >> material_data.albedo.g >> material_data.albedo.b;
            }
            else if (param == "emissive")
            {
                iss >> material_data.emissive.r >> material_data.emissive.g >> material_data.emissive.b;
            }
            else if (param == "roughness")
            {
                iss >> material_data.roughness;
            }
            else if (param == "metallic")
            {
                iss >> material_data.metallic;
            }
            else if (param == "tilling")
            {
                iss >> material_data.tilling;
            }
            else if (param == "ambient_occlusion")
            {
                iss >> material_data.ambient_occlusion;
            }
            else if (param == "emissive_strength")
            {
                iss >> material_data.emissive_strength;
            }
        }
    }

    void material_loader::parse_material_textures_from_file(const std::string &line, std::ifstream &material_file, material_data &material_data)
    {
        RT_PROFILE_SECTION("material_loader::parse_material_textures_from_file");
        std::string texture_section_line;
        std::string current_texture_type;

        // Extract the texture type from the line we pass in the params
        if (line.substr(0, line.size() - 1).back() == ':')
        {
            current_texture_type = line.substr(0, line.size() - 2);
        }

        std::getline(material_file, texture_section_line);

        // Skip empty lines and comments
        if (texture_section_line.empty() || texture_section_line[0] == '#')
            return;

        if (texture_section_line.find("path") != std::string::npos)
        {
            // Extract the texture path
            std::string texture_path = texture_section_line.substr(texture_section_line.find(":") + 1);
            texture_path = utils::trim(texture_path);
            texture_path = utils::remove_carriage(texture_path);

            // Remove leading and trailing spaces
            texture_path.erase(0, texture_path.find_first_not_of(" \t"));
            texture_path.erase(texture_path.find_last_not_of(" \t") + 1);

            // Convert texture type string to material_texture_type enum
            material_texture_type texture_type = material::get_material_texture_type_from_string(current_texture_type);

            // Create and store the material_texture object
            material_texture material_texture;
            std::string file_name = utils::remove_carriage(utils::extract_file_name(texture_path));
            auto texture_asset = assets::asset_manager::get().get_asset<texture, assets::asset_type::texture>(file_name);
            // If the texture was previously loaded use that one
            if (texture_asset)
            {
                RT_TRACE("Material has texture asset '{}' already loaded!",
                         material::get_material_texture_type_to_string(texture_type));
                material_texture.texture = texture_asset;
            }
            else
            {
                RT_TRACE("Material texture asset '{}' created because it wasnt found!",
                         material::get_material_texture_type_to_string(texture_type));
                material_texture.texture = texture_loader::load_texture_from_file(texture_path);
            }
            material_texture.type = texture_type;
            material_texture.is_enabled = true;

            material_data.textures.insert({texture_type, material_texture});
        }
    }
}