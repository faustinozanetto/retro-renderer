#include "enemies_manager.h"

#include <glm/ext/matrix_transform.hpp>

#include "game_manager.h"
#include "logging/logger.h"

enemies_manager::enemies_manager()
{
    initialize_enemy_params();
    initialize_enemy_assets();
    initialize_enemy_model();
    initialize_enemy_generation();

    initialize_enemy_waves();
    start_enemy_wave();
}

void enemies_manager::draw_enemies()
{
    m_enemy_shader->bind();
    m_enemy_shader->set_mat4("u_view", game_manager::get().get_camera()->get_view_matrix());
    m_enemy_shader->set_mat4("u_projection", game_manager::get().get_camera()->get_projection_matrix());
    for (enemy& enemy : m_enemies)
    {
        glm::mat4 enemy_model = glm::mat4(1.0f);
        enemy_model = translate(enemy_model, enemy.position);
        enemy_model = scale(enemy_model, enemy.size);
        m_enemy_shader->set_mat4("u_transform", enemy_model);
        retro::renderer::renderer::bind_texture(0, m_enemy_texture->get_handle_id());
        retro::renderer::renderer::submit_model(m_enemy_model);
    }
    m_enemy_shader->un_bind();
}

void enemies_manager::update_enemies()
{
    // Enemy behaviour
    for (enemy& enemy : m_enemies)
    {
        enemy.position.x -= enemy.speed.x * retro::core::time::get_delta_time();
        enemy.collider.position = enemy.position;
        // Did it reach the leftmost part of the screen?
        if (enemy.position.x < game_manager::get().get_level_manager()->get_level_min().x)
        {
            enemy.position = {m_enemy_rand_x(m_enemy_rand_gen), m_enemy_rand_y(m_enemy_rand_gen), 0.0f};
        }
    }
}

void enemies_manager::check_wave_finished()
{
    if (m_enemies.empty())
    {
        move_to_next_wave();
    }
}

void enemies_manager::play_enemy_explode_sound(enemy& enemy)
{
    m_enemy_sound_emitter->set_location(enemy.position);
    m_enemy_sound_emitter->set_sound(m_enemy_explode_sound);
    m_enemy_sound_emitter->play();
}

void enemies_manager::initialize_enemy_waves()
{
    m_current_wave = 0;
    m_total_waves = 5;
    for (int i = 1; i <= m_total_waves; i++)
    {
        enemies_wave wave = {i * 5};
        m_waves.push_back(wave);
    }
}

void enemies_manager::start_enemy_wave()
{
    RT_TRACE("Starting enemy wave #{} with enemey count {}", m_current_wave, m_waves[m_current_wave].enemy_count);
    generate_enemies(m_waves[m_current_wave].enemy_count);
    game_manager::get().get_level_manager()->generate_ammo_pickups(5);
}

void enemies_manager::move_to_next_wave()
{
    if (m_current_wave < m_total_waves)
    {
        m_current_wave++;
        start_enemy_wave();
    }
}

void enemies_manager::initialize_enemy_params()
{
    m_enemy_sound_emitter = std::make_shared<retro::audio::sound_emitter>();
    m_enemy_sound_emitter->set_volume(0.25f);
}

void enemies_manager::initialize_enemy_assets()
{
    m_enemy_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/enemy.png");

    m_enemy_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/enemy.rrs");
    m_enemy_explode_sound = std::make_shared<retro::audio::sound>("resources/audio/explosion.ogg");
}

void enemies_manager::initialize_enemy_model()
{
    m_enemy_model = retro::renderer::model_loader::load_model_from_file("resources/models/enemy.obj");
}

void enemies_manager::initialize_enemy_generation()
{
    // Create a random number generator
    m_enemy_rand_gen = std::mt19937(m_enemy_rd());
    m_enemy_rand_x = std::uniform_real_distribution(
        game_manager::get().get_level_manager()->get_level_max().x - 15.0f,
        game_manager::get().get_level_manager()->get_level_max().x + 25.0f);
    m_enemy_rand_y = std::uniform_real_distribution(
        game_manager::get().get_level_manager()->get_level_min().y + 1.0f,
        game_manager::get().get_level_manager()->get_level_max().y - 1.0f);
}

void enemies_manager::generate_enemies(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        enemy enemy_created;
        enemy_created.position = {m_enemy_rand_x(m_enemy_rand_gen), m_enemy_rand_y(m_enemy_rand_gen), 0.0f};
        enemy_created.speed = {12.5f, 0.0f, 0.0f};
        enemy_created.size = glm::vec3(2.5f);
        enemy_created.collider = {enemy_created.position, enemy_created.size};
        m_enemies.push_back(enemy_created);
    }
}
