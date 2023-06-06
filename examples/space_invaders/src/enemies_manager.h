#pragma once

#include "retro.h"
#include "utils.h"

#include <random>
#include <glm/glm.hpp>

struct enemy
{
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 size;

    box_collider collider;
};

class enemies_manager
{
public:
    enemies_manager();

    void draw_enemies(const std::shared_ptr<retro::renderer::shader>& geometry_shader);
    void update_enemies();

    void play_enemy_explode_sound(enemy& enemy);

    void initialize_enemy_params();
    void initialize_enemy_assets();
    void initialize_enemy_model();
    void initialize_enemy_generation();

    void generate_enemies(int amount = 25);

    void save_assets() const;

    /* Getters */
    std::vector<enemy>& get_enemies() { return m_enemies; }

private:
    /* Enemies */
    std::vector<enemy> m_enemies;
    std::shared_ptr<retro::audio::sound_emitter> m_enemy_sound_emitter;
    std::shared_ptr<retro::audio::sound> m_enemy_explode_sound;

    /* Model */
    std::shared_ptr<retro::renderer::material> m_enemy_material;
    std::shared_ptr<retro::renderer::model> m_enemy_model;
    std::shared_ptr<retro::renderer::texture> m_enemy_albedo_texture;
    std::shared_ptr<retro::renderer::texture> m_enemy_normal_texture;
    std::shared_ptr<retro::renderer::texture> m_enemy_roughness_texture;
    std::shared_ptr<retro::renderer::texture> m_enemy_metallic_texture;
    std::shared_ptr<retro::renderer::texture> m_enemy_ao_texture;
    std::shared_ptr<retro::renderer::texture> m_enemy_emissive_texture;

    /* Generation */
    std::random_device m_enemy_rd;
    std::uniform_real_distribution<float> m_enemy_rand_x;
    std::uniform_real_distribution<float> m_enemy_rand_y;
    std::mt19937 m_enemy_rand_gen;
};
