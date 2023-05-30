#include "space_invaders_app.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <imgui.h>
#include <string>
#include <format>
#include <random>

space_invaders_app::space_invaders_app() : application("./")
{
    initialize_fonts();
    initialize_camera();
    initialize_shaders();
    initialize_player();
}

space_invaders_app::~space_invaders_app()
{
}

void space_invaders_app::on_update()
{
    draw_game();
}

void space_invaders_app::process_input()
{
    
}

void space_invaders_app::draw_game()
{
    // Draw player
    m_game_shader->bind();
    m_game_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_game_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_player.position, 0.0f));
    model = glm::scale(model, {1.0f,1.0f,1.0f});
    m_game_shader->set_mat4("u_transform", model);
    m_player_vao->bind();
    retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
    m_player_vao->un_bind();
    m_game_shader->un_bind();
}

void space_invaders_app::initialize_player()
{
    m_player.position = glm::vec2(0);

    m_player_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_player_vao->bind();

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

    const std::shared_ptr<retro::renderer::vertex_buffer_object> ebo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements);
    ebo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, indices.size() * sizeof(&indices[0]),
                  indices.data());

    const std::shared_ptr<retro::renderer::vertex_buffer_object> vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
    vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, vertices.size() * sizeof(&vertices[0]),
                  vertices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}
        };

    const std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor> vbo_layout_descriptor = std::make_shared<
        retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    vbo->set_layout_descriptor(vbo_layout_descriptor);

    m_player_vao->add_vertex_buffer(vbo);
    m_player_vao->set_index_buffer(ebo);
    m_player_vao->un_bind();
}

void space_invaders_app::initialize_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.0f, 25.0f});
}

void space_invaders_app::initialize_shaders()
{
    const std::string& shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "resources/shaders/game.rrs");
    const auto& shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_game_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void space_invaders_app::initialize_fonts()
{
    m_font = std::make_shared<retro::renderer::font>("resources/fonts/arial.ttf");
}

retro::core::application* retro::core::create_application()
{
    return new space_invaders_app();
}
