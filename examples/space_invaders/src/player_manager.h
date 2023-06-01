#pragma once

#include <glm/glm.hpp>

#include "retro.h"
#include "utils.h"

struct player
{
    glm::vec2 position;
    glm::vec2 target_position;
    glm::vec2 speed;

    float move_speed;
    
    int ammo;
    int max_ammo;

    box_collider collider;
};

struct bullet
{
    glm::vec2 position;
    glm::vec2 speed;

    box_collider collider;
};

class player_manager
{
public:
    player_manager();

    void draw_player();
    void draw_bullets();

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
    
    /* Getters */
    player& get_player() { return m_player; }
    std::list<bullet>& get_bullets() { return m_player_bullets; }

private:
    /* Player */
    player m_player;

    /* Assets */
    std::shared_ptr<retro::renderer::texture> m_player_texture;
    std::shared_ptr<retro::renderer::vertex_array_object> m_player_vao;
    std::shared_ptr<retro::renderer::vertex_array_object> m_bullet_vao;
    std::shared_ptr<retro::renderer::shader> m_player_shader;
    std::shared_ptr<retro::renderer::shader> m_bullet_shader;   

    /* Bullets */
    std::list<bullet> m_player_bullets;
};
