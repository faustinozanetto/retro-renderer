#include "enemies_manager.h"

#include <glm/ext/matrix_transform.hpp>

#include "game_manager.h"
#include "audio/sound_loader.h"
#include "logging/logger.h"

enemies_manager::enemies_manager()
{
    initialize_enemy_params();
    initialize_enemy_assets();
    initialize_enemy_model();
    initialize_enemy_generation();
}

void enemies_manager::draw_enemies(const std::shared_ptr<retro::renderer::shader>& geometry_shader)
{
    for (enemy& enemy : m_enemies)
    {
        auto enemy_model = glm::mat4(1.0f);
        enemy_model = translate(enemy_model, enemy.position);
        enemy_model = scale(enemy_model, enemy.size);
        geometry_shader->set_mat4("u_transform", enemy_model);
        m_enemy_material->bind(geometry_shader);
        retro::renderer::renderer::submit_model(m_enemy_model);
    }
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
    m_enemy_albedo_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<
        retro::renderer::texture>("enemy_albedo.jpg");
    m_enemy_normal_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<
        retro::renderer::texture>("enemy_normal.jpg");
    m_enemy_roughness_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<
        retro::renderer::texture>("enemy_roughness.jpg");
    m_enemy_metallic_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<
        retro::renderer::texture>("enemy_metallic.jpg");
    m_enemy_ao_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<
        retro::renderer::texture>("enemy_ao.jpg");
    m_enemy_emissive_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<
    retro::renderer::texture>("enemy_emissive.jpg");
    
    m_enemy_model = retro::assets::asset_manager::get().get_asset_pack("models")->get_asset<retro::renderer::model>("enemy.obj");
    m_enemy_explode_sound = retro::assets::asset_manager::get().get_asset_pack("sounds")->get_asset<retro::audio::sound>("explosion.ogg");
#else
    m_enemy_albedo_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/enemy/enemy_albedo.jpg");
    m_enemy_normal_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/enemy/enemy_normal.jpg");
    m_enemy_roughness_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/enemy/enemy_roughness.jpg");
    m_enemy_metallic_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/enemy/enemy_metallic.jpg");
    m_enemy_ao_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/enemy/enemy_ao.jpg");
    m_enemy_emissive_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/enemy/enemy_emissive.jpg");
    
    m_enemy_model = retro::renderer::model_loader::load_model_from_file("resources/models/enemy.obj");
    m_enemy_explode_sound = retro::audio::sound_loader::load_sound_from_file("resources/audio/explosion.ogg");
#endif
#endif
}

void enemies_manager::initialize_enemy_model()
{
    /* Setup enemy material for model */
    retro::renderer::material_texture albedo;
    albedo.texture = m_enemy_albedo_texture;
    albedo.is_enabled = true;
    albedo.type = retro::renderer::material_texture_type::albedo;

    retro::renderer::material_texture normal;
    normal.texture = m_enemy_normal_texture;
    normal.is_enabled = true;
    normal.type = retro::renderer::material_texture_type::normal;

    retro::renderer::material_texture roughness;
    roughness.texture = m_enemy_roughness_texture;
    roughness.is_enabled = true;
    roughness.type = retro::renderer::material_texture_type::roughness;

    retro::renderer::material_texture metallic;
    metallic.texture = m_enemy_metallic_texture;
    metallic.is_enabled = true;
    metallic.type = retro::renderer::material_texture_type::metallic;

    retro::renderer::material_texture ambient_occlusion;
    ambient_occlusion.texture = m_enemy_ao_texture;
    ambient_occlusion.is_enabled = true;
    ambient_occlusion.type = retro::renderer::material_texture_type::ambient_occlusion;

    retro::renderer::material_texture emissive;
    emissive.texture = m_enemy_emissive_texture;
    emissive.is_enabled = true;
    emissive.type = retro::renderer::material_texture_type::emissive;

    std::map<retro::renderer::material_texture_type, int> material_bindings;
    material_bindings[retro::renderer::material_texture_type::albedo] = 0;
    material_bindings[retro::renderer::material_texture_type::normal] = 1;
    material_bindings[retro::renderer::material_texture_type::roughness] = 2;
    material_bindings[retro::renderer::material_texture_type::metallic] = 3;
    material_bindings[retro::renderer::material_texture_type::ambient_occlusion] = 4;
    material_bindings[retro::renderer::material_texture_type::emissive] = 5;

    std::unordered_map<retro::renderer::material_texture_type, retro::renderer::material_texture> textures;
    textures[retro::renderer::material_texture_type::albedo] = albedo;
    textures[retro::renderer::material_texture_type::normal] = normal;
    textures[retro::renderer::material_texture_type::roughness] = roughness;
    textures[retro::renderer::material_texture_type::metallic] = metallic;
    textures[retro::renderer::material_texture_type::ambient_occlusion] = ambient_occlusion;
    textures[retro::renderer::material_texture_type::emissive] = emissive;

    m_enemy_material = std::make_shared<retro::renderer::material>(textures, material_bindings);
    m_enemy_material->set_emissive_strength(1.0f);
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
        enemy_created.size = glm::vec3(4.5f);
        enemy_created.collider = {enemy_created.position, enemy_created.size};
        m_enemies.push_back(enemy_created);
    }
}

void enemies_manager::save_assets() const
{
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_enemy_albedo_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_enemy_normal_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_enemy_roughness_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_enemy_metallic_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_enemy_ao_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_enemy_emissive_texture);
    retro::assets::asset_manager::get().get_asset_pack("models")->save_asset(
        m_enemy_model);
    retro::assets::asset_manager::get().get_asset_pack("sounds")->save_asset(
        m_enemy_explode_sound);
}
