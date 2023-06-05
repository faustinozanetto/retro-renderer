﻿#include "enemies_manager.h"

#include <glm/ext/matrix_transform.hpp>

#include "game_manager.h"
#include "audio/sound_loader.h"
#include "logging/logger.h"

enemies_manager::enemies_manager()
{
    initialize_enemy_params();
    initialize_enemy_assets();
    initialize_enemy_generation();
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

void enemies_manager::play_enemy_explode_sound(enemy& enemy)
{
    m_enemy_sound_emitter->set_location(enemy.position);
    m_enemy_sound_emitter->set_sound(m_enemy_explode_sound);
    m_enemy_sound_emitter->play();
}

void enemies_manager::initialize_enemy_params()
{
    m_enemy_sound_emitter = std::make_shared<retro::audio::sound_emitter>();
    m_enemy_sound_emitter->set_volume(0.25f);
}

void enemies_manager::initialize_enemy_assets()
{
#ifdef ASSETS_FROM_PACK
#if (ASSETS_FROM_PACK == 1)
    m_enemy_texture = retro::assets::asset_manager::get().get_asset_pack(retro::assets::asset_type::texture)->
                                          get_asset<retro::renderer::texture, retro::assets::asset_type::texture>(
                                              "enemy.jpg");
    m_enemy_shader = retro::assets::asset_manager::get().get_asset_pack(retro::assets::asset_type::shader)->
                                         get_asset<retro::renderer::shader, retro::assets::asset_type::shader>(
                                             "enemy.rrs");
    m_enemy_model = retro::assets::asset_manager::get().get_asset_pack(retro::assets::asset_type::model)->
                                        get_asset<retro::renderer::model,
                                                  retro::assets::asset_type::model>("enemy.obj");
    m_enemy_explode_sound = retro::assets::asset_manager::get().get_asset_pack(retro::assets::asset_type::sound)->
                                                get_asset<retro::audio::sound,
                                                          retro::assets::asset_type::sound>("explosion.ogg");
#else
    m_enemy_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/enemy.jpg");
    m_enemy_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/enemy.rrs");
    m_enemy_model = retro::renderer::model_loader::load_model_from_file("resources/models/enemy.obj");
    m_enemy_explode_sound = retro::audio::sound_loader::load_sound_from_file("resources/audio/explosion.ogg");
#endif
#endif
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

void enemies_manager::save_assets() const
{
    retro::assets::asset_manager::get().get_asset_pack(retro::assets::asset_type::texture)->save_asset(
        m_enemy_texture);
    retro::assets::asset_manager::get().get_asset_pack(retro::assets::asset_type::model)->save_asset(
        m_enemy_model);
    retro::assets::asset_manager::get().get_asset_pack(retro::assets::asset_type::shader)->save_asset(
        m_enemy_shader);
    retro::assets::asset_manager::get().get_asset_pack(retro::assets::asset_type::sound)->save_asset(
    m_enemy_explode_sound);
}
