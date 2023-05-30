#pragma once

#include "retro.h"

struct player
{
    glm::vec2 position;
};

class space_invaders_app : public retro::core::application
{
public:
    space_invaders_app();
    ~space_invaders_app() override;

    void on_update() override;

    void process_input();
    void draw_game();

    void initialize_player();
    void initialize_camera();
    void initialize_fonts();
    void initialize_shaders();
private:
    /* Player */
    player m_player;
    std::shared_ptr<retro::renderer::vertex_array_object> m_player_vao;

    /* Camera */
    std::shared_ptr<retro::camera::camera> m_camera;

    /* Common */
    std::shared_ptr<retro::renderer::shader> m_game_shader;

    /* Fonts */
    std::shared_ptr<retro::renderer::font> m_font;
};
