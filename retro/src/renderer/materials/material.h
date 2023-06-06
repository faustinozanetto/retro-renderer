#pragma once

#include "renderer/textures/texture.h"
#include "renderer/shaders/shader.h"

#include <map>

namespace retro::renderer
{
    enum class material_texture_type
    {
        albedo,
        normal,
        roughness,
        metallic,
        ambient_occlusion,
        emissive
    };

    struct material_texture
    {
        std::shared_ptr<texture> texture;
        material_texture_type type;
        bool is_enabled;
    };

    class material
    {
    public:
        material(const std::map<material_texture_type, int>& texture_bindings);
        material(const std::unordered_map<material_texture_type, material_texture>& textures,
                 const std::map<material_texture_type, int>& texture_bindings);
        ~material();

        /* Functions */
        void bind(const std::shared_ptr<shader>& shader);

        /* Getters */
        glm::vec3 get_albedo() const { return m_albedo; }
        glm::vec3 get_emissive() const { return m_emissive; }
        float get_roughness() const { return m_roughness; }
        float get_metallic() const { return m_metallic; }
        float get_tilling() const { return m_tilling; }
        float get_ambient_occlusion() const { return m_ambient_occlusion; }
        float get_emissive_strength() const { return m_emissive_strength; }

        /* Setters */
        void set_albedo(const glm::vec3& albedo) { m_albedo = albedo; }
        void set_emissive(const glm::vec3& emissive) { m_emissive = emissive; }
        void set_roughness(float roughness) { m_roughness = roughness; }
        void set_metallic(float metallic) { m_metallic = metallic; }
        void set_ambient_occlusion(float ambient_occlusion) { m_ambient_occlusion = ambient_occlusion; }
        void set_tilling(float tilling) { m_tilling = tilling; }
        void set_emissive_strength(float emissive_strength) { m_emissive_strength = emissive_strength; }

        /* Utilities */
        static std::string get_material_texture_type_to_string(material_texture_type texture_type);
        static std::string get_material_enabled_uniform_location(material_texture_type texture_type);

    private:
        std::unordered_map<material_texture_type, material_texture> m_textures;
        std::map<material_texture_type, int> m_material_texture_bindings;

        glm::vec3 m_albedo;
        glm::vec3 m_emissive;
        float m_roughness;
        float m_metallic;
        float m_tilling;
        float m_ambient_occlusion;
        float m_emissive_strength;
    };
}
