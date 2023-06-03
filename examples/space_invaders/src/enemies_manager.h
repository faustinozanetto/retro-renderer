#pragma once

#include "retro.h"
#include "utils.h"

#include <random>
#include <glm/glm.hpp>

#define ASSETS_FROM_PACK 1

struct enemy
{
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 size;

    box_collider collider;
};

struct enemies_wave
{
    int enemy_count;
};

class enemies_manager
{
public:
    enemies_manager();

    void draw_enemies();
    void update_enemies();

    void check_wave_finished();

    void play_enemy_explode_sound(enemy&enemy);

    void initialize_enemy_waves();
    void start_enemy_wave();
    void move_to_next_wave();

    void initialize_enemy_params();
    void initialize_enemy_assets();
    void initialize_enemy_model();
    void initialize_enemy_generation();

    void generate_enemies(int amount = 25);

    void save_assets() const;
    
    /* Getters */
    std::vector<enemy>& get_enemies() { return m_enemies; }
    int get_current_wave() const { return m_current_wave; }
    int get_total_waves() const { return m_total_waves; }

private:
    /* Enemies */
    std::shared_ptr<retro::renderer::texture> m_enemy_texture;
    std::vector<enemy> m_enemies;
    std::shared_ptr<retro::renderer::shader> m_enemy_shader;
    std::shared_ptr<retro::audio::sound_emitter> m_enemy_sound_emitter;
    std::shared_ptr<retro::audio::sound> m_enemy_explode_sound;
    std::shared_ptr<retro::renderer::model> m_enemy_model;

    /* Waves */
    int m_total_waves;
    int m_current_wave;
    std::vector<enemies_wave> m_waves;

    /* Generation */
    std::random_device m_enemy_rd;
    std::uniform_real_distribution<float> m_enemy_rand_x;
    std::uniform_real_distribution<float> m_enemy_rand_y;
    std::mt19937 m_enemy_rand_gen;
};
