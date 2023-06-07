#include "level_manager.h"

#include <glm/ext/matrix_transform.hpp>

#include "game_manager.h"
#include "audio/sound_loader.h"

level_manager *level_manager::s_instance = nullptr;

level_manager::level_manager()
{
    s_instance = this;
    initialize_level_params();
    initialize_background_assets();
    initialize_background_model();
    initialize_ammo_pickup_assets();
    initialize_ammo_pickup_generation();
    initialize_audio();
}

void level_manager::draw_background(const std::shared_ptr<retro::renderer::shader> &geometry_shader)
{
    m_background_vao->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, {0, 0, -5.0f});
    model = glm::scale(model, {500.0f, 500.0f, 1.0f});
    geometry_shader->set_mat4("u_transform", model);
    m_background_material->bind(geometry_shader);
    retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
    m_background_vao->un_bind();
}

void level_manager::draw_ammo_pickups(const std::shared_ptr<retro::renderer::shader> &geometry_shader)
{
    for (ammo_pickup &ammo_pickup : m_ammo_pickups)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = translate(model, ammo_pickup.position);
        model = scale(model, ammo_pickup.size);
        model = rotate(model, retro::core::time::get_time() * 5, {0, 1, 0});
        geometry_shader->set_mat4("u_transform", model);
        m_ammo_pickup_material->bind(geometry_shader);
        retro::renderer::renderer::submit_model(m_ammo_pickup_model);
    }
}

void level_manager::update_ammo_pickups()
{
    // Enemy behaviour
    for (ammo_pickup &ammo_pickup : m_ammo_pickups)
    {
        ammo_pickup.position.x -= ammo_pickup.speed.x * retro::core::time::get_delta_time();
        ammo_pickup.collider.position = ammo_pickup.position;
        // Did it reach the leftmost part of the screen?
        if (ammo_pickup.position.x < m_level_min.x)
        {
            ammo_pickup.position = {
                m_ammo_pickups_rand_x(m_ammo_pickups_rand_gen), m_ammo_pickups_rand_y(m_ammo_pickups_rand_gen), 0.0f};
        }
    }
}

void level_manager::play_ambient_sound()
{
    m_ambient_sound_emitter->set_location(game_manager::get().get_player_manager()->get_player().position);
    m_ambient_sound_emitter->set_sound(m_ambient_sound);
}

void level_manager::initialize_audio()
{
    m_ammo_pickup_sound_emitter = std::make_shared<retro::audio::sound_emitter>();
    m_ambient_sound_emitter = std::make_shared<retro::audio::sound_emitter>();
}

void level_manager::initialize_level_params()
{
    m_level_min = {-2.0f, -13.0f};
    m_level_max = {80.0f, 13.0f};
}

void level_manager::initialize_background_assets()
{
#ifdef ASSETS_FROM_PACK
#if (ASSETS_FROM_PACK == 1)
    m_ambient_sound = retro::assets::asset_manager::get().get_asset_pack(
                                                             "sounds")
                          ->get_asset<retro::audio::sound>(
                              "ambient.wav");
    m_background_material = retro::assets::asset_manager::get().get_asset_pack(
                                                                   "materials")
                                ->get_asset<retro::renderer::material>(
                                    "background.rrm");
#else
    m_ambient_sound = retro::audio::sound_loader::load_sound_from_file("resources/audio/ambient.wav");
    m_background_material = retro::renderer::material_loader::load_material_from_file("resources/materials/background.rrm");
#endif
#endif
}

void level_manager::initialize_background_model()
{
    m_background_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_background_vao->bind();
    const std::vector<float> vertices = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    const std::vector<uint32_t> indices = {
        0, 3, 1, // first triangle
        1, 3, 2, // second triangle
    };

    const auto ebo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements);
    ebo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw,
                  indices.size() * sizeof(&indices[0]),
                  indices.data());

    const auto vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
    vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw,
                  vertices.size() * sizeof(&vertices[0]),
                  vertices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}};

    const auto vbo_layout_descriptor = std::make_shared<
        retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    vbo->set_layout_descriptor(vbo_layout_descriptor);

    m_background_vao->add_vertex_buffer(vbo);
    m_background_vao->set_index_buffer(ebo);
    m_background_vao->un_bind();
}

void level_manager::initialize_ammo_pickup_assets()
{
#ifdef ASSETS_FROM_PACK
#if (ASSETS_FROM_PACK == 1)

    m_ammo_pickup_material = retro::assets::asset_manager::get().get_asset_pack("materials")->get_asset<retro::renderer::material>("ammo_pickup.rrm");

    m_ammo_pickup_model = retro::assets::asset_manager::get().get_asset_pack(
                                                                 "models")
                              ->get_asset<retro::renderer::model>(
                                  "ammo_pickup.fbx");
    m_ammo_pickup_sound = retro::assets::asset_manager::get().get_asset_pack(
                                                                 "sounds")
                              ->get_asset<retro::audio::sound>(
                                  "ammo_pickup.ogg");
#else
    m_ammo_pickup_material = retro::renderer::material_loader::load_material_from_file("resources/materials/ammo_pickup.rrm");
    m_ammo_pickup_model = retro::renderer::model_loader::load_model_from_file("resources/models/ammo_pickup.fbx");
    m_ammo_pickup_sound = retro::audio::sound_loader::load_sound_from_file("resources/audio/ammo_pickup.ogg");
#endif
#endif
}

void level_manager::initialize_ammo_pickup_generation()
{
    m_ammo_pickups = std::list<ammo_pickup>();

    // Create a random number generator
    m_ammo_pickups_rand_gen = std::mt19937(m_ammo_pickups_rd());
    m_ammo_pickups_rand_x = std::uniform_real_distribution(
        m_level_max.x - 15.0f,
        m_level_max.x + 25.0f);
    m_ammo_pickups_rand_y = std::uniform_real_distribution(
        m_level_min.y + 1.1f,
        m_level_max.y - 1.0f);
}

void level_manager::clear_ammo_pickups()
{
    m_ammo_pickups.clear();
}

void level_manager::generate_ammo_pickups(int count)
{
    for (int i = 0; i < count; i++)
    {
        ammo_pickup pikcup;
        pikcup.position = {
            m_ammo_pickups_rand_x(m_ammo_pickups_rand_gen), m_ammo_pickups_rand_y(m_ammo_pickups_rand_gen), 0.0f};
        pikcup.speed = {10.5f, 0.0f, 0.0f};
        pikcup.size = glm::vec3(0.05f);
        pikcup.collider = {pikcup.position, glm::vec3(1.0f)};
        m_ammo_pickups.push_back(pikcup);
    }
}

void level_manager::play_ammo_pickup_sound()
{
    m_ammo_pickup_sound_emitter->set_location(game_manager::get().get_player_manager()->get_player().position);
    m_ammo_pickup_sound_emitter->set_sound(m_ammo_pickup_sound);
    m_ammo_pickup_sound_emitter->play();
}

void level_manager::save_assets() const
{
    auto &materials_asset_pack = retro::assets::asset_manager::get().get_asset_pack("materials");
    auto &textures_asset_pack = retro::assets::asset_manager::get().get_asset_pack("textures");
    materials_asset_pack->save_asset(m_ammo_pickup_material);
    m_ammo_pickup_material->save_textures(textures_asset_pack);
    materials_asset_pack->save_asset(m_background_material);
    m_background_material->save_textures(textures_asset_pack);

    retro::assets::asset_manager::get().get_asset_pack("models")->save_asset(
        m_ammo_pickup_model);
    retro::assets::asset_manager::get().get_asset_pack("sounds")->save_asset(
        m_ambient_sound);
    retro::assets::asset_manager::get().get_asset_pack("sounds")->save_asset(
        m_ammo_pickup_sound);
}
