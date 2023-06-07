#include "player_manager.h"

#include <glm/ext/matrix_transform.hpp>

#include "game_manager.h"
#include "audio/sound_loader.h"

#define LERP(a, b, t) a * (1.0 - t) + (b * t)

player_manager::player_manager()
{
    initialize_player_params();
    initialize_player_assets();
    initialize_player_model();
}

void player_manager::draw_player(const std::shared_ptr<retro::renderer::shader> &geometry_shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = translate(model, m_player.position);
    model = scale(model, m_player.collider.size);
    geometry_shader->set_mat4("u_transform", model);
    m_player_material->bind(geometry_shader);
    retro::renderer::renderer::submit_model(m_player_model);
}

void player_manager::draw_bullets(const std::shared_ptr<retro::renderer::shader> &geometry_shader)
{
    for (bullet &bullet : m_player_bullets)
    {
        auto model = glm::mat4(1.0f);
        model = translate(model, bullet.position);
        model = scale(model, {2.0f, 0.15f, 1.0f});
        geometry_shader->set_mat4("u_transform", model);
        m_bullet_vao->bind();
        m_bullet_material->bind(geometry_shader);
        retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
        m_bullet_vao->un_bind();
    }
}

void player_manager::update_player()
{
    m_player.collider.position = m_player.position;
    m_player_sound_emitter->set_location(m_player.position);
}

void player_manager::update_bullets()
{
    for (auto it = m_player_bullets.begin(); it != m_player_bullets.end();)
    {
        it->position.x += it->speed.x * retro::core::time::get_delta_time();
        it->collider.position = it->position;
        if (it->position.x > game_manager::get().get_level_manager()->get_level_max().x)
        {
            it = m_player_bullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void player_manager::refill_ammo()
{
    m_player.ammo = m_player.max_ammo;
}

void player_manager::lerp_player_position()
{
    // Lineally interpolate current to target player location.
    float lerp_player_y = LERP(m_player.position.y,
                               m_player.target_position.y,
                               retro::core::time::get_delta_time() * m_player.move_speed);
    lerp_player_y = std::clamp(lerp_player_y, game_manager::get().get_level_manager()->get_level_min().y,
                               game_manager::get().get_level_manager()->get_level_max().y);
    m_player.position.y = lerp_player_y;
}

void player_manager::initialize_player_params()
{
    m_player_bullets = std::list<bullet>();
    m_player_sound_emitter = std::make_shared<retro::audio::sound_emitter>();

    m_player.max_ammo = 10;
    m_player.ammo = m_player.max_ammo;
    m_player.move_speed = 6.5f;
    m_player.position = glm::vec3(3.0f, 0.0f, 0.0f);
    m_player.target_position = m_player.position;
    m_player.speed = glm::vec3(5.0f);
    m_player.collider = box_collider(m_player.position, glm::vec3(3.0f));
}

void player_manager::initialize_player_assets()
{
#ifdef ASSETS_FROM_PACK
#if (ASSETS_FROM_PACK == 1)
    m_player_albedo_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<retro::renderer::texture>("player_albedo.png");
    m_player_normal_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<retro::renderer::texture>("player_normal.png");
    m_player_roughness_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<retro::renderer::texture>("player_roughness.png");
    m_player_metallic_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<retro::renderer::texture>("player_metallic.png");
    m_player_ao_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<retro::renderer::texture>("player_ao.png");
    m_player_emissive_texture = retro::assets::asset_manager::get().get_asset_pack("textures")->get_asset<retro::renderer::texture>("player_emissive.png");

    m_player_model = retro::assets::asset_manager::get().get_asset_pack("models")->get_asset<retro::renderer::model>("player.obj");
    m_shoot_sound = retro::assets::asset_manager::get().get_asset_pack("sounds")->get_asset<retro::audio::sound>(
        "player_shoot.wav");
    m_crash_sound = retro::assets::asset_manager::get().get_asset_pack("sounds")->get_asset<retro::audio::sound>(
        "explosion.ogg");
    m_player_material = retro::assets::asset_manager::get().get_asset_pack("materials")->get_asset<retro::renderer::material>("player.rrm");
    m_bullet_material = retro::assets::asset_manager::get().get_asset_pack("materials")->get_asset<retro::renderer::material>("bullet.rrm");
#else
    m_player_albedo_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/player/player_albedo.png");
    m_player_normal_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/player/player_normal.png");
    m_player_roughness_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/player/player_roughness.png");
    m_player_metallic_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/player/player_metallic.png");
    m_player_ao_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/player/player_ao.png");
    m_player_emissive_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/player/player_emissive.png");

    m_player_model = retro::renderer::model_loader::load_model_from_file(
        "resources/models/player.obj");
    m_shoot_sound = retro::audio::sound_loader::load_sound_from_file("resources/audio/player_shoot.wav");
    m_crash_sound = retro::audio::sound_loader::load_sound_from_file("resources/audio/explosion.ogg");

    m_player_material = retro::renderer::material_loader::load_material_from_file("resources/materials/player.rrm");
    m_bullet_material = retro::renderer::material_loader::load_material_from_file("resources/materials/bullet.rrm");
#endif
#endif
}

void player_manager::initialize_player_model()
{
    {
        m_bullet_vao = std::make_shared<retro::renderer::vertex_array_object>();
        m_bullet_vao->bind();

        const std::vector<float> vertices = {
            // Positions      // Texture coordinates
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-right
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // Top-right
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // Top-left
        };

        const std::vector<uint32_t> indices = {
            0, 1, 2, // First triangle
            2, 3, 0  // Second triangle
        };

        const auto ebo = std::make_shared<
            retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements);
        ebo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, indices.size() * sizeof(&indices[0]),
                      indices.data());

        const auto vbo = std::make_shared<
            retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
        vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, vertices.size() * sizeof(&vertices[0]),
                      vertices.data());

        std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
            layout_elements = {
                {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
                {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}};

        const auto vbo_layout_descriptor = std::make_shared<
            retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
        vbo->set_layout_descriptor(vbo_layout_descriptor);

        m_bullet_vao->add_vertex_buffer(vbo);
        m_bullet_vao->set_index_buffer(ebo);
        m_bullet_vao->un_bind();
    }
}

void player_manager::move_player_up()
{
    if (m_player.position.y < game_manager::get().get_level_manager()->get_level_max().y)
        m_player.target_position.y += m_player.speed.y;
    else
        m_player.target_position.y = game_manager::get().get_level_manager()->get_level_max().y;
}

void player_manager::move_player_down()
{
    if (m_player.position.y > game_manager::get().get_level_manager()->get_level_min().y)
        m_player.target_position.y -= m_player.speed.y;
    else
        m_player.target_position.y = game_manager::get().get_level_manager()->get_level_min().y;
}

void player_manager::player_shoot()
{
    if (m_player.ammo == 0)
        return;

    m_player_sound_emitter->set_sound(m_shoot_sound);
    m_player_sound_emitter->play();

    bullet player_bullet;
    player_bullet.speed = glm::vec3(120.0f);
    player_bullet.position = m_player.position;
    player_bullet.collider.position = m_player.position;
    player_bullet.collider.size = glm::vec3(1.0f);
    m_player_bullets.push_back(player_bullet);
    m_player.ammo--;
}

void player_manager::player_crash()
{
    m_player_sound_emitter->set_sound(m_crash_sound);
    m_player_sound_emitter->play();
}

void player_manager::save_assets() const
{
    /* Save player textures */
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_player_albedo_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_player_normal_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_player_roughness_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_player_metallic_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_player_ao_texture);
    retro::assets::asset_manager::get().get_asset_pack("textures")->save_asset(m_player_emissive_texture);

    retro::assets::asset_manager::get().get_asset_pack("materials")->save_asset(m_player_material);
    retro::assets::asset_manager::get().get_asset_pack("materials")->save_asset(m_bullet_material);

    retro::assets::asset_manager::get().get_asset_pack("models")->save_asset(
        m_player_model);
    retro::assets::asset_manager::get().get_asset_pack("sounds")->save_asset(
        m_crash_sound);
    retro::assets::asset_manager::get().get_asset_pack("sounds")->save_asset(
        m_shoot_sound);
}
