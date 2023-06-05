#pragma once

#include "retro.h"
#include "utils.h"

#include <random>
#include <glm/glm.hpp>

struct ammo_pickup
{
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 size;

    box_collider collider;
};

class level_manager
{
public:
    level_manager();

    void draw_background();
    void draw_ammo_pickups();
    void update_ammo_pickups();

    void play_ambient_sound();

    void initialize_audio();
    void initialize_level_params();
    void initialize_background_assets();
    void initialize_background_model();
    void initialize_ammo_pickup_assets();
    void initialize_ammo_pickup_generation();

    void clear_ammo_pickups();
    void generate_ammo_pickups(int count = 5);
    void play_ammo_pickup_sound();

    void save_assets() const;
    
    /* Getters */
    static level_manager& get() { return *s_instance; }
    
    glm::vec2 get_level_min() const { return m_level_min; }
    glm::vec2 get_level_max() const { return m_level_max; }
    std::list<ammo_pickup>& get_ammo_pickups() { return m_ammo_pickups; }

private:
    /* Common */
    glm::vec2 m_level_min;
    glm::vec2 m_level_max;
    std::shared_ptr<retro::audio::sound_emitter> m_ammo_pickup_sound_emitter;
    std::shared_ptr<retro::audio::sound_emitter> m_ambient_sound_emitter;

    /* Assets */
    std::shared_ptr<retro::renderer::shader> m_background_shader;
    std::shared_ptr<retro::renderer::shader> m_ammo_pickup_shader;
    std::shared_ptr<retro::renderer::texture> m_background_texture;
    std::shared_ptr<retro::renderer::vertex_array_object> m_background_vao;
    std::shared_ptr<retro::audio::sound> m_ammo_pickup_sound;
    std::shared_ptr<retro::audio::sound> m_ambient_sound;

    /* Ammo Pickups */
    std::shared_ptr<retro::renderer::model> m_ammo_pickup_model;
    std::shared_ptr<retro::renderer::texture> m_ammo_pickup_texture;
    std::list<ammo_pickup> m_ammo_pickups;
    std::random_device m_ammo_pickups_rd;
    std::uniform_real_distribution<float> m_ammo_pickups_rand_x;
    std::uniform_real_distribution<float> m_ammo_pickups_rand_y;
    std::mt19937 m_ammo_pickups_rand_gen;

    static level_manager* s_instance;
};
