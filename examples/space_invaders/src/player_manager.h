#pragma once

#include <glm/glm.hpp>

#include "retro.h"
#include "utils.h"

struct player
{
    glm::vec3 position;
    glm::vec3 target_position;
    glm::vec3 speed;

    float move_speed;

    int ammo;
    int max_ammo;

    box_collider collider;
};

struct bullet
{
    glm::vec3 position;
    glm::vec3 speed;

    box_collider collider;
};

class player_manager
{
public:
    player_manager();

    void draw_player(const std::shared_ptr<retro::renderer::shader>& geometry_shader);
    void draw_bullets(const std::shared_ptr<retro::renderer::shader>& geometry_shader);

    void update_player();
    void update_bullets();

    void refill_ammo();

    void lerp_player_position();

    void initialize_player_params();
    void initialize_player_assets();
    void initialize_player_model();

    void move_player_up();
    void move_player_down();
    void player_shoot();
    void player_crash();

    void save_assets() const;

    /* Getters */
    player &get_player() { return m_player; }
    std::list<bullet> &get_bullets() { return m_player_bullets; }

private:
    /* Player */
    player m_player;
    std::shared_ptr<retro::audio::sound_emitter> m_player_sound_emitter;

    /* Model */
    std::shared_ptr<retro::renderer::material> m_player_material;
    std::shared_ptr<retro::renderer::model> m_player_model;
    std::shared_ptr<retro::renderer::texture> m_player_albedo_texture;
    std::shared_ptr<retro::renderer::texture> m_player_normal_texture;
    std::shared_ptr<retro::renderer::texture> m_player_roughness_texture;
    std::shared_ptr<retro::renderer::texture> m_player_metallic_texture;
    std::shared_ptr<retro::renderer::texture> m_player_ao_texture;

    /* Assets */
    std::shared_ptr<retro::renderer::vertex_array_object> m_bullet_vao;
    std::shared_ptr<retro::audio::sound> m_shoot_sound;
    std::shared_ptr<retro::audio::sound> m_crash_sound;

    /* Bullets */
    std::list<bullet> m_player_bullets;
};
