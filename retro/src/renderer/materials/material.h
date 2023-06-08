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

        material_data() : textures({}), albedo(glm::vec3(0.0f)), emissive(glm::vec3(0.0f)), roughness(1.0f), metallic(0.0f), ambient_occlusion(1.0f), emissive_strength(0.0f), tilling(1.0f) {}

        material_data(const std::map<material_texture_type, material_texture> &textures, const glm::vec3 &albedo = glm::vec3(0.0f), const glm::vec3 &emissive = glm::vec3(0.0f), float roughness = 1.0f, float metallic = 0.0f, float ambient_occlusion = 1.0f, float emissive_strength = 0.0f, float tilling = 1.0f) : textures(textures), albedo(albedo), emissive(emissive), roughness(roughness), metallic(metallic), ambient_occlusion(ambient_occlusion), emissive_strength(emissive_strength), tilling(tilling) {}
    };

    constexpr material_texture_type types[] = {
        material_texture_type::albedo,
        material_texture_type::normal,
        material_texture_type::roughness,
        material_texture_type::metallic,
        material_texture_type::ambient_occlusion,
        material_texture_type::emissive,
        material_texture_type::opacity};

    class material : public assets::asset
    {
    public:
        material(const std::string &file_name, const material_data &material_data,
                 const std::map<material_texture_type, int> &texture_bindings);
        ~material();

        /* Functions */
        void bind(const std::shared_ptr<shader> &shader);

        /* Getters */
        const material_data &get_data() const { return m_data; }

        /* Asset */
        void save_textures(const std::shared_ptr<assets::asset_pack> &asset_pack);
        void serialize(std::ofstream &asset_pack_file) override;
        static std::shared_ptr<material> deserialize(const assets::asset_metadata &metadata, std::ifstream &asset_pack_file);

        /* Utilities */
        static material_texture_type get_material_texture_type_from_string(const std::string &texture_type);
        static std::string get_material_texture_type_to_string(material_texture_type texture_type);
        static std::string get_material_enabled_uniform_location(material_texture_type texture_type);

    private:
        std::map<material_texture_type, int> m_material_texture_bindings;
        material_data m_data;
    };
}
