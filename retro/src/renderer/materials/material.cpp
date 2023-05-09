#include "rtpch.h"
#include "material.h"

#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
    material::material(const std::unordered_map<material_texture_type, material_texture> &textures, const std::map<material_texture_type, int> &texture_bindings)
    {
        const material_texture_type types[] = {
            material_texture_type::albedo,
            material_texture_type::normal,
            material_texture_type::roughness,
            material_texture_type::metallic,
            material_texture_type::ambient_occlusion};

        for (const material_texture_type type : types)
        {
            material_texture texture;
            texture.is_enabled = false;
            texture.type = type;
            texture.texture = nullptr;
            m_textures.insert({type, texture});
        }

        for (auto &[type, texture] : textures)
        {
            m_textures[type] = texture;
        }

        m_material_texture_bindings = texture_bindings;
        m_albedo = glm::vec3(0.15f);
        m_roughness = 1.0f;
        m_metallic = 0.0f;
        m_ambient_occlusion = 1.0f;
        m_tilling = 1.0f;
    }

    material::~material()
    {
    }

    void material::bind(const std::shared_ptr<shader> &shader)
    {
        shader->set_vec_float3("u_material.albedo", m_albedo);
        shader->set_float("u_material.roughness", m_roughness);
        shader->set_float("u_material.metallic", m_metallic);
        shader->set_float("u_material.ambient_occlusion", m_ambient_occlusion);
        shader->set_float("u_material.tilling", m_tilling);

        for (auto &[type, texture] : m_textures)
        {
            std::string enabled_uniform_location = get_material_enabled_uniform_location(type);
            shader->set_int(enabled_uniform_location, texture.is_enabled ? 1 : 0);
            if (!texture.is_enabled)
                continue;
            int bind_slot = m_material_texture_bindings[type];
            renderer::bind_texture(bind_slot, texture.texture->get_handle_id());
        }
    }

    std::string material::get_material_texture_type_to_string(material_texture_type texture_type)
    {
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
        }
        RT_ASSERT_MSG(false, "Invalid material texture type!");
    }

    std::string material::get_material_enabled_uniform_location(material_texture_type texture_type)
    {
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
        }
        RT_ASSERT_MSG(false, "Invalid material texture type!");
    }
}