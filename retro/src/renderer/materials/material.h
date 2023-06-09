#pragma once

#include "renderer/textures/texture.h"
#include "renderer/shaders/shader.h"
#include "assets/asset_pack.h"

namespace retro::renderer
{
    enum class material_texture_type
    {
        albedo,
        normal,
        roughness,
        metallic,
        ambient_occlusion,
        emissive,
        opacity
    };

    struct material_texture
    {
        std::shared_ptr<texture> texture;
        material_texture_type type;
        bool is_enabled;
    };

    struct material_data_serialize
    {
        glm::vec3 albedo;
        glm::vec3 emissive;
        float roughness;
        float metallic;
        float ambient_occlusion;
        float emissive_strength;
        float tilling;
    };

    struct material_data
    {
        glm::vec3 albedo;
        glm::vec3 emissive;
        float roughness;
        float metallic;
        float ambient_occlusion;
        float emissive_strength;
        float tilling;

        std::map<material_texture_type, material_texture> textures;

        material_data() : albedo(glm::vec3(0.0f)), emissive(glm::vec3(0.0f)), roughness(1.0f), metallic(0.0f),
                          ambient_occlusion(1.0f), emissive_strength(0.0f), tilling(1.0f), textures({}) {}

        material_data(const std::map<material_texture_type, material_texture> &textures, const glm::vec3 &albedo = glm::vec3(0.0f), const glm::vec3 &emissive = glm::vec3(0.0f), float roughness = 1.0f, float metallic = 0.0f, float ambient_occlusion = 1.0f, float emissive_strength = 0.0f, float tilling = 1.0f) : textures(textures), albedo(albedo), emissive(emissive), roughness(roughness), metallic(metallic), ambient_occlusion(ambient_occlusion), emissive_strength(emissive_strength), tilling(tilling) {}
    };

   constexpr material_texture_type material_texture_types_array[] = {
        material_texture_type::albedo,
        material_texture_type::normal,
        material_texture_type::roughness,
        material_texture_type::metallic,
        material_texture_type::ambient_occlusion,
        material_texture_type::emissive,
        material_texture_type::opacity};

    static material_data material_default_data = {};

    class material : public assets::asset
    {
    public:
        material(const std::string &file_name, const material_data &material_data,
                 const std::map<material_texture_type, int> &texture_bindings);
        ~material() override;

        /* Functions */
        void bind(const std::shared_ptr<shader> &shader);
        static void bind_default(const std::shared_ptr<shader> &shader);

        /* Getters */
        const material_texture& get_material_texture(material_texture_type texture_type) const { return m_data.textures.at(texture_type); }
        const std::map<material_texture_type, material_texture>& get_material_textures() const { return m_data.textures; }
		const glm::vec3& get_albedo() const { return m_data.albedo; }
		const glm::vec3& get_emissive() const { return m_data.emissive; }
		float get_ambient_occlusion() const { return m_data.ambient_occlusion; }
		float get_emissive_strength() const { return m_data.emissive_strength; }
		float get_tilling() const { return m_data.tilling; }
		float get_roughness() const { return m_data.roughness; }
		float get_metallic() const { return m_data.metallic; }

        /* Setters */
        void set_texture(const material_texture& material_texture);
        void set_texture_enabled(material_texture_type texture_type, bool is_enabled);
        void set_albedo(const glm::vec3 &albedo) { m_data.albedo = albedo; }
        void set_emissive(const glm::vec3 & emissive) { m_data.emissive = emissive; }
        void set_ambient_occlusion(float ambient_occlusion) { m_data.ambient_occlusion = ambient_occlusion; }
        void set_emissive_strength(float emissive_strength) { m_data.emissive_strength = emissive_strength; }
        void set_tilling(float tilling) { m_data.tilling = tilling; }
        void set_roughness(float roughness) { m_data.roughness = roughness; }
        void set_metallic(float metallic) { m_data.metallic = metallic; }

        /* Asset */
        void save_textures(const std::shared_ptr<assets::asset_pack> &asset_pack) const;
        void serialize(std::ofstream &asset_pack_file) override;
        static std::shared_ptr<material> deserialize(const assets::asset_metadata &metadata, std::ifstream &asset_pack_file);

        /* Utilities */
        static std::vector<material_texture_type> get_material_texture_types();
        static material_texture_type get_material_texture_type_from_string(const std::string &texture_type);
        static const char* get_material_texture_type_to_string(material_texture_type texture_type);

    private:
        std::map<material_texture_type, int> m_material_texture_bindings;
        std::map<material_texture_type, std::string> m_material_texture_enabled_locations;
        material_data m_data;
    };
}
