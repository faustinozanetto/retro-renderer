#pragma once

#include <random>

#include "retro.h"
#include <glm/glm.hpp>
#include <queue>

struct box_collider
{
    glm::vec2 position;
    glm::vec2 size;
};

struct player
{
    glm::vec2 position;
    glm::vec2 target_position;
    glm::vec2 speed;

    box_collider collider;
};

struct bullet
{
    glm::vec2 position;
    glm::vec2 speed;

    box_collider collider;
};

struct enemy
{
    glm::vec2 position;
    glm::vec2 speed;
    bool alive;

    box_collider collider;
};

enum class game_state
{
    start,
    play,
    end,
};

class space_invaders_app : public retro::core::application
{
public:
    space_invaders_app();
    ~space_invaders_app() override;

    void on_update() override;

    void interpolate_positions();
    void update_enemies();
    void update_bullets();
    void update_colliders();
    void handle_collisions();
    void check_game_end();
    void draw_game();

    void reset_enemy_position(enemy& enemy);

    void initialize_level();
    void initialize_texts();
    void initialize_debug();
    void initialize_enemies();
    void initialize_player();
    void initialize_bullet();
    void initialize_camera();
    void initialize_fonts();
    void initialize_shaders();

    bool check_collider_collision(box_collider& collider1, box_collider& collider2);
    
    void on_handle_event(retro::events::base_event& event) override;
    bool on_window_resize(retro::events::window_resize_event& window_resize_event);
    bool on_key_pressed(retro::events::key_pressed_event& key_pressed_event);

private:
    /* Player */
    player m_player;
    float m_player_move_speed;
    std::shared_ptr<retro::renderer::vertex_array_object> m_player_vao;
    std::shared_ptr<retro::renderer::vertex_array_object> m_bullet_vao;
    std::list<bullet> m_player_bullets;

    /* Game */
    game_state m_game_state;

    /* Camera */
    std::shared_ptr<retro::camera::camera> m_camera;

    /* Level */
    float m_level_min_y;
    float m_level_max_y;
    float m_level_min_x;
    float m_level_max_x;

    std::random_device m_enemy_rd;
    std::uniform_real_distribution<float> m_enemy_rand_x;
    std::uniform_real_distribution<float> m_enemy_rand_y;
    std::mt19937 m_enemy_rand_gen;

    /* Text */
    std::shared_ptr<retro::renderer::text> m_end_text;
    std::shared_ptr<retro::renderer::text> m_fps_text;
    std::shared_ptr<retro::renderer::text> m_enemies_left_text;       
    std::shared_ptr<retro::renderer::shader> m_text_shader;

    /* Debug */
    std::shared_ptr<retro::renderer::vertex_array_object> m_collider_debug_vao;

    /* Enemies */
    std::vector<enemy> m_enemies;
    std::shared_ptr<retro::renderer::vertex_array_object> m_enemy_vao;

    /* Common */
    std::shared_ptr<retro::renderer::shader> m_game_shader;

    /* Fonts */
    std::shared_ptr<retro::renderer::font> m_font;
};
