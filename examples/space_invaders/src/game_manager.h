#pragma once

#include "player_manager.h"
#include "level_manager.h"
#include "enemies_manager.h"

enum class game_state
{
    starting,
    playing,
    ending,
};

struct game_wave
{
    int enemy_count;
    int ammo_pickup_count;
};

class game_manager
{
public:
    game_manager();

    void start_game();
    
    void update_game();
    void draw_game();

    void debug_asset_packs();

    void handle_collisions();
    void check_game_end();
    void initialize_managers();
    
    void initialize_camera();
    void initialize_fonts();
    void initialize_shaders();
    void initialize_texts();

    void check_wave_finished();
    void initialize_game_waves();
    void start_game_wave() const;
    void move_to_next_wave();

    bool on_key_pressed(retro::events::key_pressed_event& key_pressed_event);
    bool on_window_resize(retro::events::window_resize_event& window_resize_event);

    void save_assets();

    /* Getters */
    static game_manager& get() { return *s_instance; }
    std::shared_ptr<retro::camera::camera>& get_camera() { return m_camera; }
    std::shared_ptr<player_manager>& get_player_manager() { return m_player_manager;}
    std::shared_ptr<level_manager>& get_level_manager() { return m_level_manager;}
    std::shared_ptr<enemies_manager>& get_enemies_manager() { return m_enemies_manager;}
    int get_current_wave() const { return m_current_wave; }
    int get_total_waves() const { return m_total_waves; }
    
    static bool check_collider_collision(const box_collider& collider1, const box_collider& collider2);

private:
    /* Game */
    game_state m_game_state;
    std::shared_ptr<player_manager> m_player_manager;
    std::shared_ptr<level_manager> m_level_manager;
    std::shared_ptr<enemies_manager> m_enemies_manager;
    bool m_disable_player_collision;
    
    /* Waves */
    int m_total_waves;
    int m_current_wave;
    std::vector<game_wave> m_waves;

    /* Texts */
    std::shared_ptr<retro::renderer::text> m_fps_text;
    std::shared_ptr<retro::renderer::text> m_ammo_text;
    std::shared_ptr<retro::renderer::text> m_wave_text;
    std::shared_ptr<retro::renderer::text> m_enemies_left_text;
    
    /* Fonts */
    std::shared_ptr<retro::renderer::font> m_font;
    std::shared_ptr<retro::renderer::shader> m_font_shader;
    
    /* Camera */
    std::shared_ptr<retro::camera::camera> m_camera;

    static game_manager* s_instance;
};
