#include "level_manager.h"

#include <glm/ext/matrix_transform.hpp>

#include "game_manager.h"

level_manager* level_manager::s_instance = nullptr;

level_manager::level_manager()
{
    s_instance = this;
    initialize_level_params();
    initialize_background_assets();
    initialize_background_model();
    initialize_ammo_pickup_assets();
    initialize_ammo_pickup_model();
    initialize_ammo_pickup_generation();

    generate_ammo_pickups(5);
}

void level_manager::draw_background()
{
    retro::renderer::renderer::set_depth_test(false);
    m_background_shader->bind();
    m_background_vao->bind();
    retro::renderer::renderer::bind_texture(0, m_background_texture->get_handle_id());
    retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
    m_background_vao->un_bind();
    m_background_shader->un_bind();
    retro::renderer::renderer::set_depth_test(true);
}

void level_manager::draw_ammo_pickups()
{
    m_ammo_pickup_shader->bind();
    m_ammo_pickup_shader->set_mat4("u_view", game_manager::get().get_camera()->get_view_matrix());
    m_ammo_pickup_shader->set_mat4("u_projection", game_manager::get().get_camera()->get_projection_matrix());
    for (ammo_pickup& ammo_pickup : m_ammo_pickups)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = translate(model, glm::vec3(ammo_pickup.position, 0.0f));
        model = scale(model, {ammo_pickup.size, 1.0f});
        m_ammo_pickup_shader->set_mat4("u_transform", model);
        m_ammo_pickup_vao->bind();
        retro::renderer::renderer::bind_texture(0, m_ammo_pickup_texture->get_handle_id());
        retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
        m_ammo_pickup_vao->un_bind();
    }
    m_ammo_pickup_shader->un_bind();
}

void level_manager::update_ammo_pickups()
{
    // Enemy behaviour
    for (ammo_pickup& ammo_pickup : m_ammo_pickups)
    {
        ammo_pickup.position.x -= ammo_pickup.speed.x * retro::core::time::get_delta_time();
        ammo_pickup.collider.position = ammo_pickup.position;
        // Did it reach the leftmost part of the screen?
        if (ammo_pickup.position.x < m_level_min.x)
        {
            ammo_pickup.position = {
                m_ammo_pickups_rand_x(m_ammo_pickups_rand_gen), m_ammo_pickups_rand_y(m_ammo_pickups_rand_gen)
            };
        }
    }
}

void level_manager::initialize_level_params()
{
    m_level_min = {-2.0f, -13.0f};
    m_level_max = {80.0f, 13.0f};
}

void level_manager::initialize_background_assets()
{
    m_background_texture = retro::renderer::texture_loader::load_texture_from_file(
        "resources/textures/test_texture2.png");

    const std::string& shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "resources/shaders/background.rrs");
    const auto& shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_background_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void level_manager::initialize_background_model()
{
    m_background_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_background_vao->bind();
    const std::vector<float> vertices = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
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
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}
        };

    const auto vbo_layout_descriptor = std::make_shared
    <
        retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    vbo->set_layout_descriptor(vbo_layout_descriptor);

    m_background_vao->add_vertex_buffer(vbo);
    m_background_vao->set_index_buffer(ebo);
    m_background_vao->un_bind();
}

void level_manager::initialize_ammo_pickup_assets()
{
    m_ammo_pickup_texture = retro::renderer::texture_loader::load_texture_from_file(
        "resources/textures/ammo_pickup.png");

    const std::string& shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "resources/shaders/ammo_pickup.rrs");
    const auto& shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_ammo_pickup_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void level_manager::initialize_ammo_pickup_model()
{
    m_ammo_pickup_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_ammo_pickup_vao->bind();

    const std::vector<float> vertices = {
        // Positions      // Texture coordinates
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // Top-right
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f // Top-left
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2, // First triangle
        2, 3, 0 // Second triangle
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
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}
        };

    const auto vbo_layout_descriptor = std::make_shared<
        retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    vbo->set_layout_descriptor(vbo_layout_descriptor);

    m_ammo_pickup_vao->add_vertex_buffer(vbo);
    m_ammo_pickup_vao->set_index_buffer(ebo);
    m_ammo_pickup_vao->un_bind();
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
        m_level_max.y + 1.0f,
        m_level_min.y - 1.0f);
}

void level_manager::generate_ammo_pickups(int count)
{
    for (int i = 0; i < count; i++)
    {
        ammo_pickup pikcup;
        pikcup.position = {
            m_ammo_pickups_rand_x(m_ammo_pickups_rand_gen), m_ammo_pickups_rand_y(m_ammo_pickups_rand_gen)
        };
        pikcup.speed = {10.5f, 10.5f};
        pikcup.size = {1.5f, 1.5f};
        pikcup.collider = {pikcup.position, pikcup.size};
        m_ammo_pickups.push_back(pikcup);
    }
}
