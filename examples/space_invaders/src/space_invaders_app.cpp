#include "space_invaders_app.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <imgui.h>
#include <string>
#include <format>
#include <random>

#include "logging/logger.h"

#define LERP(a, b, t) a * (1.0 - t) + (b * t)

space_invaders_app::space_invaders_app() : application("./")
{
    initialize_level();
    initialize_fonts();
    initialize_camera();
    initialize_shaders();
    initialize_bullet();
    initialize_player();
    initialize_enemies();
    initialize_texts();
    initialize_debug();
    retro::renderer::renderer::set_vsync_enabled(false);
    glLineWidth(1.5f);
    m_game_state = game_state::play;
}

space_invaders_app::~space_invaders_app()
{
}

void space_invaders_app::on_update()
{
    if (m_game_state == game_state::play)
    {
        interpolate_positions();
        update_colliders();
        update_enemies();
        update_bullets();
        handle_collisions();
        check_game_end();
        draw_game();
    }
    else if (m_game_state == game_state::end)
    {
        m_text_shader->bind();
        retro::renderer::renderer::submit_text(m_text_shader, m_font, m_end_text);
        m_text_shader->un_bind();
    }
    /*
        retro::ui::interface::begin_frame();
    
    
        ImGui::Begin("Space Invaders");
        ImGui::Text("Player Pos (%.3f, %.3f)", m_player.position.x, m_player.position.y);
        ImGui::Text("Player Target Pos (%.3f, %.3f)", m_player.target_position.x, m_player.target_position.y);
        ImGui::Text("Collider Pos (%.3f, %.3f)", m_player.collider.position.x, m_player.collider.position.y);
        ImGui::Text("Enemy Count %d", m_enemies.size());
        ImGui::Text("Game State %s",
                    m_game_state == game_state::start ? "Start" : m_game_state == game_state::play ? "Play" : "End");
        ImGui::Text("Player Bullet Count %d", m_player_bullets.size());
        ImGui::End();
        retro::ui::interface::end_frame();*/
}

void space_invaders_app::interpolate_positions()
{
    // Lineally interpolate current to target player location.
    float lerp_player_y = LERP(m_player.position.y,
                               m_player.target_position.y,
                               retro::core::time::get_delta_time() * m_player_move_speed);
    lerp_player_y = std::clamp(lerp_player_y, m_level_min_y, m_level_max_y);
    m_player.position.y = lerp_player_y;
}

void space_invaders_app::update_enemies()
{
    // Enemy behaviour
    for (enemy& enemy : m_enemies)
    {
        if (enemy.alive)
        {
            enemy.position.x -= enemy.speed.x * retro::core::time::get_delta_time();

            // Did it reach the leftmost part of the screen?
            if (enemy.position.x < m_level_min_x)
            {
                RT_INFO("Enemy reach end of screen!");
                reset_enemy_position(enemy);
            }
        }
    }
}

void space_invaders_app::update_bullets()
{
    for (auto it = m_player_bullets.begin(); it != m_player_bullets.end(); ++it)
    {
        it->position.x += it->speed.x * retro::core::time::get_delta_time();
        if (it->position.x > m_level_max_x)
        {
            RT_INFO("Bullet reach end of screen!");
            it = m_player_bullets.erase(it);
            --it; // as it will be add again in for, so we go back one step
        }
    }
}

void space_invaders_app::update_colliders()
{
    m_player.collider.position = m_player.position;
    for (enemy& enemy : m_enemies)
    {
        if (enemy.alive)
        {
            enemy.collider.position = enemy.position;
        }
    }

    for (bullet& bullet : m_player_bullets)
    {
        bullet.collider.position = bullet.position;
    }
}

void space_invaders_app::handle_collisions()
{
    // Check enemies and player collision
    for (enemy& enemy : m_enemies)
    {
        if (enemy.alive)
        {
            if (check_collider_collision(enemy.collider, m_player.collider))
            {
                m_game_state = game_state::end;
                break;
            }
        }
    }

    // Check bullet with enemies
    for (auto it = m_player_bullets.begin(); it != m_player_bullets.end(); ++it)
    {
        for (auto it_enemy = m_enemies.begin(); it_enemy != m_enemies.end(); ++it_enemy)
        {
            if (it_enemy->alive)
            {
                if (check_collider_collision(it_enemy->collider, it->collider))
                {
                    RT_INFO("Bullet killed enemy!");
                    // Handle bullet delete
                    it = m_player_bullets.erase(it);
                    --it; // as it will be add again in for, so we go back one step
                    // Handle enemy delete
                    it_enemy = m_enemies.erase(it_enemy);
                    --it_enemy;
                }
            }
        }
    }
}

void space_invaders_app::check_game_end()
{
    if (m_enemies.empty())
    {
        m_game_state = game_state::end;
    }
}

void space_invaders_app::draw_game()
{
    m_game_shader->bind();
    m_game_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_game_shader->set_mat4("u_projection", m_camera->get_projection_matrix());

    // Draw player
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = translate(model, glm::vec3(m_player.position, 0.0f));
        model = scale(model, {1.0f, 1.0f, 1.0f});
        m_game_shader->set_mat4("u_transform", model);
        m_game_shader->set_vec_float3("u_color", {0.25f, 0.34f, 0.65f});
        m_player_vao->bind();
        retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
        m_player_vao->un_bind();
    }

    // Draw enemies
    {
        for (enemy& enemy : m_enemies)
        {
            glm::mat4 enemy_model = glm::mat4(1.0f);
            enemy_model = translate(enemy_model, glm::vec3(enemy.position, 0.0f));
            enemy_model = scale(enemy_model, {1.0f, 1.0f, 1.0f});
            m_game_shader->set_mat4("u_transform", enemy_model);
            m_game_shader->set_vec_float3("u_color", {0.5f, 0.4f, 0.8f});
            m_enemy_vao->bind();
            retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
            m_enemy_vao->un_bind();
        }
    }

    // Draw bullets
    {
        for (bullet& bullet : m_player_bullets)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = translate(model, glm::vec3(bullet.position, 0.0f));
            model = scale(model, {1.0f, 0.15f, 1.0f});
            m_game_shader->set_mat4("u_transform", model);
            m_game_shader->set_vec_float3("u_color", {0.15f, 0.85f, 0.35f});
            m_bullet_vao->bind();
            retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
            m_bullet_vao->un_bind();
        }
    }

    // Debug draw colliders
    m_game_shader->set_vec_float3("u_color", {0.8f, 0.8f, 0.8f});
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = translate(model, glm::vec3(m_player.collider.position, 0.1f));
        model = scale(model, {1.0f, 1.0f, 1.0f});
        m_game_shader->set_mat4("u_transform", model);
        m_collider_debug_vao->bind();
        retro::renderer::renderer::submit_elements(GL_LINE_LOOP, 6);
        m_collider_debug_vao->un_bind();
    }
    {
        for (enemy& enemy : m_enemies)
        {
            glm::mat4 enemy_model = glm::mat4(1.0f);
            enemy_model = translate(enemy_model, glm::vec3(enemy.collider.position, 0.0f));
            enemy_model = scale(enemy_model, {1.0f, 1.0f, 1.0f});
            m_game_shader->set_mat4("u_transform", enemy_model);
            m_collider_debug_vao->bind();
            retro::renderer::renderer::submit_elements(GL_LINE_LOOP, 6);
            m_collider_debug_vao->un_bind();
        }
    }

    m_game_shader->un_bind();

    // Texts
    m_text_shader->bind();
    m_enemies_left_text->set_content(std::format("Enemies Left: {}", m_enemies.size()));
    retro::renderer::renderer::submit_text(m_text_shader, m_font, m_enemies_left_text);
    m_text_shader->un_bind();

    /*
    // Draw text
    m_text_shader->bind();
    ImGuiIO& io = ImGui::GetIO();
    float framerate = io.Framerate;
    m_fps_text->set_content(std::format("FPS: {}", framerate));
    retro::renderer::renderer::submit_text(m_text_shader, m_font, m_fps_text);
    m_text_shader->un_bind();
    */
}

void space_invaders_app::reset_enemy_position(enemy& enemy)
{
    enemy.position = {m_enemy_rand_x(m_enemy_rand_gen), m_enemy_rand_y(m_enemy_rand_gen)};
}

void space_invaders_app::initialize_level()
{
    m_level_min_y = -13.0f;
    m_level_max_y = 13.0f;
    m_level_min_x = -2.0f;
    m_level_max_x = 80.0f;

    float enemy_span_x = 80.0f;

    // Create a random number generator
    m_enemy_rand_gen = std::mt19937(m_enemy_rd());
    m_enemy_rand_x = std::uniform_real_distribution(enemy_span_x - 10.0f, enemy_span_x + 10.0f);
    m_enemy_rand_y = std::uniform_real_distribution(m_level_min_y + 1.0f, m_level_max_y - 1.0f);

    for (int i = 0; i < 25; i++)
    {
        enemy enemy_created;
        reset_enemy_position(enemy_created);
        enemy_created.speed = {12.5f, 12.5f};
        enemy_created.alive = true;
        enemy_created.collider = {enemy_created.position, {1.0f, 1.0f}};
        m_enemies.push_back(enemy_created);
    }
}

void space_invaders_app::initialize_texts()
{
    m_fps_text = std::make_shared<retro::renderer::text>("FPS", glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.5f));
    m_end_text = std::make_shared<retro::renderer::text>("Game End!",
                                                         glm::vec2(retro::renderer::renderer::get_viewport_size().x / 2,
                                                                   retro::renderer::renderer::get_viewport_size().y /
                                                                   2), glm::vec2(0.5f, 0.5f));
    m_enemies_left_text = std::make_shared<retro::renderer::text>("Enemies Left: 0", glm::vec2(0,
                                                                      retro::renderer::renderer::get_viewport_size().y -
                                                                      25.0f),
                                                                  glm::vec2(0.5f, 0.5f));
}

void space_invaders_app::initialize_debug()
{
    // Setup debug collider vao
    {
        m_collider_debug_vao = std::make_shared<retro::renderer::vertex_array_object>();
        m_collider_debug_vao->bind();
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

        m_collider_debug_vao->add_vertex_buffer(vbo);
        m_collider_debug_vao->set_index_buffer(ebo);
        m_collider_debug_vao->un_bind();
    }
}

void space_invaders_app::initialize_enemies()
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

void space_invaders_app::initialize_player()
{
    m_player_move_speed = 6.5f;
    m_player.position = glm::vec2(0);
    m_player.target_position = m_player.position;
    m_player.speed = glm::vec2(5.0f);

    m_player.collider = box_collider(m_player.position, {1.0f, 1.0f});

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

    m_player_vao->add_vertex_buffer(vbo);
    m_player_vao->set_index_buffer(ebo);
    m_player_vao->un_bind();
}

void space_invaders_app::initialize_bullet()
{
    m_player_bullets = std::list<bullet>();
    m_bullet_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_bullet_vao->bind();

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

    m_bullet_vao->add_vertex_buffer(vbo);
    m_bullet_vao->set_index_buffer(ebo);
    m_bullet_vao->un_bind();
}

void space_invaders_app::initialize_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f,
                                                       1000.0f);
    m_camera->set_position({24.0f, 0.0f, 25.0f});
}

void space_invaders_app::initialize_shaders()
{
    {
        const std::string& shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/game.rrs");
        const auto& shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_game_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
    {
        const std::string& shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/font.rrs");
        const auto& shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_text_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
}

bool space_invaders_app::check_collider_collision(box_collider& collider1, box_collider& collider2)
{
    // Calculate the minimum and maximum coordinates for each collider
    float left1 = collider1.position.x;
    float right1 = collider1.position.x + collider1.size.x;
    float top1 = collider1.position.y;
    float bottom1 = collider1.position.y + collider1.size.y;

    float left2 = collider2.position.x;
    float right2 = collider2.position.x + collider2.size.x;
    float top2 = collider2.position.y;
    float bottom2 = collider2.position.y + collider2.size.y;

    // Check for overlap along the x-axis
    if (right1 < left2 || left1 > right2)
        return false;

    // Check for overlap along the y-axis
    if (bottom1 < top2 || top1 > bottom2)
        return false;

    // Colliders overlap in both the x and y axes
    return true;
}

void space_invaders_app::on_handle_event(retro::events::base_event& event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::key_pressed_event>(BIND_EVENT_FN(space_invaders_app::on_key_pressed));
    dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(space_invaders_app::on_window_resize));
}

bool space_invaders_app::on_window_resize(retro::events::window_resize_event& window_resize_event)
{
    m_end_text->set_posiiton(glm::vec2(window_resize_event.get_size().x / 2,
                                       window_resize_event.get_size().y / 2));
    m_enemies_left_text->set_posiiton(glm::vec2(0, window_resize_event.get_size().y - 25.0f));
    return false;
}

bool space_invaders_app::on_key_pressed(retro::events::key_pressed_event& key_pressed_event)
{
    // Move up
    if (key_pressed_event.get_key_code() == retro::key::W)
    {
        if (m_player.position.y < m_level_max_y)
            m_player.target_position.y += m_player.speed.y;
        else m_player.target_position.y = m_level_max_y;
    }
    // Move down
    else if (key_pressed_event.get_key_code() == retro::key::S)
    {
        if (m_player.position.y > m_level_min_y)
            m_player.target_position.y -= m_player.speed.y;
        else m_player.target_position.y = m_level_min_y;
    }
    // Shoot
    else if (key_pressed_event.get_key_code() == retro::key::Space)
    {
        bullet player_bullet;
        player_bullet.speed = glm::vec2(75.0f);
        player_bullet.position = m_player.position;
        player_bullet.collider.position = m_player.position;
        player_bullet.collider.size = glm::vec2(1.0f);
        m_player_bullets.push_back(player_bullet);
    }
    return false;
}

void space_invaders_app::initialize_fonts()
{
    m_font = std::make_shared<retro::renderer::font>("resources/fonts/arial.ttf");
}

retro::core::application* retro::core::create_application()
{
    return new space_invaders_app();
}
