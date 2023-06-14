#include "rtpch.h"
#include "material.h"

#include "renderer/renderer/renderer.h"
#include "renderer/textures/texture_loader.h"
#include "assets/asset_manager.h"

namespace retro::renderer
{
    material::material(const std::string &file_name, const material_data &material_data,
                       const std::map<material_texture_type, int> &texture_bindings) : asset({assets::asset_type::material, file_name})
    {
        m_material_texture_bindings = texture_bindings;
        m_data = material_data;

        // Fill in the textures that were not provided in the constructor
        for (material_texture_type type : material_texture_types_array)
        {
            if (m_data.textures.contains(type))
                continue;

            material_texture texture;
            texture.is_enabled = false;
            texture.type = type;
            texture.texture = nullptr;
            m_data.textures[type] = texture;
        }
    }

    material::~material()
    {
    }

    void material::bind(const std::shared_ptr<shader> &shader)
    {
        RT_PROFILE_SECTION("material::bind");
        shader->set_vec_float3("u_material.albedo", m_data.albedo);
        shader->set_vec_float3("u_material.emissive", m_data.emissive);
        shader->set_float("u_material.roughness", m_data.roughness);
        shader->set_float("u_material.metallic", m_data.metallic);
        shader->set_float("u_material.ambient_occlusion", m_data.ambient_occlusion);
        shader->set_float("u_material.tilling", m_data.tilling);
        shader->set_float("u_material.emissive_strength", m_data.emissive_strength);

        for (auto &[type, texture] : m_data.textures)
        {
            std::string enabled_uniform_location = get_material_enabled_uniform_location(type);
            shader->set_int(enabled_uniform_location, texture.is_enabled ? 1 : 0);
            if (!texture.is_enabled)
                continue;
            int bind_slot = m_material_texture_bindings[type];
            renderer::bind_texture(bind_slot, texture.texture->get_handle_id());
        }
    }

    void material::save_textures(const std::shared_ptr<assets::asset_pack> &asset_pack)
    {
        RT_PROFILE_SECTION("material::save_textures");
        for (auto &texture : m_data.textures)
        {
            if (!texture.second.texture)
                continue;

            asset_pack->save_asset(texture.second.texture);
        }
    }

	void material::set_texture(const material_texture& material_texture)
	{
        RT_PROFILE_SECTION("material::set_texture");
        m_data.textures[material_texture.type] = material_texture;
	}

	void material::set_texture_enabled(material_texture_type texture_type, bool is_enabled)
	{
        RT_PROFILE_SECTION("material::set_texture_enabled");
        m_data.textures[texture_type].is_enabled = is_enabled;
	}

    void material::serialize(std::ofstream &asset_pack_file)
    {
        RT_PROFILE_SECTION("material::serialize");
        // Serialize material data excluding the textures map
        material_data_serialize data_serialize;
        data_serialize.albedo = m_data.albedo;
        data_serialize.emissive = m_data.emissive;
        data_serialize.roughness = m_data.roughness;
        data_serialize.metallic = m_data.metallic;
        data_serialize.ambient_occlusion = m_data.ambient_occlusion;
        data_serialize.emissive_strength = m_data.emissive_strength;
        data_serialize.tilling = m_data.tilling;

        asset_pack_file.write(reinterpret_cast<const char *>(&data_serialize), sizeof(data_serialize));

        // Count textures with valid reference
        int num_textures = 0;
        for (auto &texture : m_data.textures)
        {
            if (!texture.second.texture)
                continue;
            num_textures++;
        }

        asset_pack_file.write(reinterpret_cast<const char *>(&num_textures), sizeof(int));
        for (auto &texture : m_data.textures)
        {
            if (!texture.second.texture)
                continue;

            // Serialize texture file path
            size_t file_path_length = texture.second.texture->get_metadata().file_path.size();
            asset_pack_file.write(reinterpret_cast<const char *>(&file_path_length), sizeof(file_path_length));
            asset_pack_file.write(texture.second.texture->get_metadata().file_path.c_str(), file_path_length);

            // Serialize texture type
            asset_pack_file.write(reinterpret_cast<const char *>(&texture.first), sizeof(texture.first));

            // Serialize texture enabled
            asset_pack_file.write(reinterpret_cast<const char *>(&texture.second.is_enabled), sizeof(texture.second.is_enabled));
        }

        // Serialize material texture bindings
        int num_bindings = m_material_texture_bindings.size();
        asset_pack_file.write(reinterpret_cast<const char *>(&num_bindings), sizeof(int));
        for (const auto &binding : m_material_texture_bindings)
        {
            asset_pack_file.write(reinterpret_cast<const char *>(&binding.first), sizeof(material_texture_type));
            asset_pack_file.write(reinterpret_cast<const char *>(&binding.second), sizeof(int));
        }
    }

    std::shared_ptr<material> material::deserialize(const assets::asset_metadata &metadata, std::ifstream &asset_pack_file)
    {
        RT_PROFILE_SECTION("material::deserialize");
        // Deserialize material data excluding the textures map
        material_data_serialize data_serialize;
        asset_pack_file.read(reinterpret_cast<char *>(&data_serialize), sizeof(material_data_serialize));

        // Reconstruct the material data struct
        material_data data;
        data.albedo = data_serialize.albedo;
        data.emissive = data_serialize.emissive;
        data.roughness = data_serialize.roughness;
        data.metallic = data_serialize.metallic;
        data.ambient_occlusion = data_serialize.ambient_occlusion;
        data.emissive_strength = data_serialize.emissive_strength;
        data.tilling = data_serialize.tilling;

        // Deserialize textures
        int num_textures;
        asset_pack_file.read(reinterpret_cast<char *>(&num_textures), sizeof(int));
        for (int i = 0; i < num_textures; ++i)
        {
            // Deserialize textue file path.
            std::string texture_file_path;
            size_t file_path_length;
            asset_pack_file.read(reinterpret_cast<char *>(&file_path_length), sizeof(file_path_length));
            texture_file_path.resize(file_path_length);
            asset_pack_file.read(&texture_file_path[0], file_path_length);

            // Deserialize texture type
            material_texture_type texture_type;
            asset_pack_file.read(reinterpret_cast<char *>(&texture_type), sizeof(texture_type));

            // Deserialize texture enabled
            bool texture_enabled;
            asset_pack_file.read(reinterpret_cast<char *>(&texture_enabled), sizeof(texture_enabled));

            material_texture material_texture;
            auto texture_asset = assets::asset_manager::get().get_asset<texture, assets::asset_type::texture>(utils::extract_file_name(texture_file_path));
            // If the texture was previously loaded use that one
            if (texture_asset)
            {
                RT_TRACE("Material '{}' has texture asset '{}' already loaded!", metadata.file_name, material::get_material_texture_type_to_string(texture_type));
                material_texture.texture = texture_asset;
            }
            else
            {
                material_texture.texture = texture_loader::load_texture_from_file(texture_file_path);
            }

            material_texture.type = texture_type;
            material_texture.is_enabled = texture_enabled;

            data.textures.insert({texture_type, material_texture});
        }

        // De serialize material texture bindings
        std::map<material_texture_type, int> material_bindings;

        int num_bindings;
        asset_pack_file.read(reinterpret_cast<char *>(&num_bindings), sizeof(int));
        for (int i = 0; i < num_bindings; ++i)
        {
            material_texture_type type;
            int texture_index;
            asset_pack_file.read(reinterpret_cast<char *>(&type), sizeof(material_texture_type));
            asset_pack_file.read(reinterpret_cast<char *>(&texture_index), sizeof(int));

            // Add the texture binding to the material
            material_bindings[type] = texture_index;
        }

        auto created_material = std::make_shared<material>(metadata.file_path, data, material_bindings);
        created_material->set_metadata(metadata);
        return created_material;
    }

    std::vector<material_texture_type> material::get_material_texture_types()
    {
        return { material_texture_type::albedo,
        material_texture_type::normal,
        material_texture_type::roughness,
        material_texture_type::metallic,
        material_texture_type::ambient_occlusion,
        material_texture_type::emissive,
        material_texture_type::opacity };
    }

    material_texture_type material::get_material_texture_type_from_string(const std::string &texture_type)
    {
        RT_PROFILE_SECTION("material::get_material_texture_type_from_string");
        if (texture_type == "albedo")
            return material_texture_type::albedo;
        else if (texture_type == "normal")
            return material_texture_type::normal;
        else if (texture_type == "roughness")
            return material_texture_type::roughness;
        else if (texture_type == "metallic")
            return material_texture_type::metallic;
        else if (texture_type == "ambient_occlusion")
            return material_texture_type::ambient_occlusion;
        else if (texture_type == "emissive")
            return material_texture_type::emissive;
        else if (texture_type == "opacity")
            return material_texture_type::opacity;
        RT_ASSERT_MSG(false, "Invalid material texture type!");
    }

    std::string material::get_material_texture_type_to_string(material_texture_type texture_type)
    {
        RT_PROFILE_SECTION("material::get_material_texture_type_to_string");
        switch (texture_type)
        {
        case material_texture_type::albedo:
            return "albedo";
        case material_texture_type::normal:
            return "normal";
        case material_texture_type::roughness:
            return "roughness";
        case material_texture_type::metallic:
            return "metallic";
        case material_texture_type::ambient_occlusion:
            return "ambient_occlusion";
        case material_texture_type::emissive:
            return "emissive";
        case material_texture_type::opacity:
            return "opacity";
        }
        RT_ASSERT_MSG(false, "Invalid material texture type!");
    }

    std::string material::get_material_enabled_uniform_location(material_texture_type texture_type)
    {
        RT_PROFILE_SECTION("material::get_material_enabled_uniform_location");
        switch (texture_type)
        {
        case material_texture_type::albedo:
            return "u_material.albedo_map_enabled";
        case material_texture_type::normal:
            return "u_material.normal_map_enabled";
        case material_texture_type::roughness:
            return "u_material.roughness_map_enabled";
        case material_texture_type::metallic:
            return "u_material.metallic_map_enabled";
        case material_texture_type::ambient_occlusion:
            return "u_material.ambient_occlusion_map_enabled";
        case material_texture_type::emissive:
            return "u_material.emissive_map_enabled";
        case material_texture_type::opacity:
            return "u_material.opacity_map_enabled";
        }
        RT_ASSERT_MSG(false, "Invalid material texture type!");
    }
}
