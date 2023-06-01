#include "enemies_manager.h"

#include <glm/ext/matrix_transform.hpp>

#include "game_manager.h"

enemies_manager::enemies_manager()
{
    initialize_enemy_params();
    initialize_enemy_assets();
    initialize_enemy_model();
    initialize_enemy_generation();

    generate_enemies(20);
}

void enemies_manager::draw_enemies()
{
    m_enemy_shader->bind();
    m_enemy_shader->set_mat4("u_view", game_manager::get().get_camera()->get_view_matrix());
    m_enemy_shader->set_mat4("u_projection", game_manager::get().get_camera()->get_projection_matrix());
    for (enemy& enemy : m_enemies)
    {
        glm::mat4 enemy_model = glm::mat4(1.0f);
        enemy_model = translate(enemy_model, glm::vec3(enemy.position, 0.0f));
        enemy_model = scale(enemy_model, {enemy.size, 1.0f});
        m_enemy_shader->set_mat4("u_transform", enemy_model);
        m_enemy_vao->bind();
        retro::renderer::renderer::bind_texture(0, m_enemy_texture->get_handle_id());
        retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
        m_enemy_vao->un_bind();
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
            enemy.position = {m_enemy_rand_x(m_enemy_rand_gen), m_enemy_rand_y(m_enemy_rand_gen)};
        }
    }
}

void enemies_manager::initialize_enemy_params()
{
}

void enemies_manager::initialize_enemy_assets()
{
    m_enemy_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/enemy.png");

    const std::string& shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "resources/shaders/enemy.rrs");
    const auto& shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_enemy_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void enemies_manager::initialize_enemy_model()
{
    m_enemy_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_enemy_vao->bind();

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

    m_enemy_vao->add_vertex_buffer(vbo);
    m_enemy_vao->set_index_buffer(ebo);
    m_enemy_vao->un_bind();
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
        enemy_created.position = {m_enemy_rand_x(m_enemy_rand_gen), m_enemy_rand_y(m_enemy_rand_gen)};
        enemy_created.speed = {12.5f, 12.5f};
        enemy_created.size = glm::vec2(2.5f);
        enemy_created.collider = {enemy_created.position, enemy_created.size};
        m_enemies.push_back(enemy_created);
    }
}
