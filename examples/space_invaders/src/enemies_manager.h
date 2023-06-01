#pragma once

#include "retro.h"
#include "utils.h"

#include <random>
#include <glm/glm.hpp>

struct enemy
{
    glm::vec2 position;
    glm::vec2 speed;
    glm::vec2 size;

    box_collider collider;
};

class enemies_manager
{
public:
    enemies_manager();

    void draw_enemies();
    void update_enemies();

    void initialize_enemy_params();
    void initialize_enemy_assets();
    void initialize_enemy_model();
    void initialize_enemy_generation();

    void generate_enemies(int amount = 25);

    /* Getters */
    inline std::vector<enemy>& get_enemies() { return m_enemies; }

private:
    /* Enemies */
    std::shared_ptr<retro::renderer::texture> m_enemy_texture;
    std::vector<enemy> m_enemies;
    std::shared_ptr<retro::renderer::vertex_array_object> m_enemy_vao;
    std::shared_ptr<retro::renderer::shader> m_enemy_shader;

    /* Generation */
    std::random_device m_enemy_rd;
    std::uniform_real_distribution<float> m_enemy_rand_x;
    std::uniform_real_distribution<float> m_enemy_rand_y;
    std::mt19937 m_enemy_rand_gen;
};
