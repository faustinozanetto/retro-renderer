#include "game_manager.h"

game_manager* game_manager::s_instance = nullptr;

game_manager::game_manager()
{
    s_instance = this;
    m_game_state = game_state::playing;
    retro::renderer::renderer::set_vsync_enabled(false);
    initialize_assets();
#ifdef ASSETS_FROM_PACK
#if (ASSETS_FROM_PACK == 1)
    m_assets_manager->deserialize_packs();
#else
#endif
#endif

    initialize_camera();
    initialize_shaders();
    initialize_fonts();
    initialize_texts();
    initialize_audio();
    initialize_managers();
}

void game_manager::start_game()
{
    m_game_state = game_state::playing;
    m_level_manager->play_ambient_sound();
}

void game_manager::update_game()
{
    if (m_game_state == game_state::playing)
    {
        m_player_manager->lerp_player_position();
        m_player_manager->update_player();
        m_player_manager->update_bullets();
        m_enemies_manager->update_enemies();
        m_enemies_manager->check_wave_finished();
        m_level_manager->update_ammo_pickups();

        handle_collisions();

        check_game_end();

        draw_game();
    }
}

void game_manager::draw_game()
{
    m_level_manager->draw_background();
    m_level_manager->draw_ammo_pickups();
    m_player_manager->draw_player();
    m_player_manager->draw_bullets();
    m_enemies_manager->draw_enemies();

    // Texts
    m_font_shader->bind();
    m_enemies_left_text->set_content(std::format("Enemies Left: {}", m_enemies_manager->get_enemies().size()));
    retro::renderer::renderer::submit_text(m_font_shader, m_font, m_enemies_left_text);

    m_ammo_text->set_content(std::format("Ammo Left: {}", m_player_manager->get_player().ammo));
    retro::renderer::renderer::submit_text(m_font_shader, m_font, m_ammo_text);

    m_fps_text->set_content(std::format("FPS: {}", retro::core::time::get_fps()));
    retro::renderer::renderer::submit_text(m_font_shader, m_font, m_fps_text);
    m_font_shader->un_bind();
}

void game_manager::handle_collisions()
{
    // 1. Check enemy-player collisions
    for (const enemy& enemy : m_enemies_manager->get_enemies())
    {
        if (check_collider_collision(enemy.collider, m_player_manager->get_player().collider))
        {
            m_game_state = game_state::ending;
            m_player_manager->player_crash();
            break;
        }
    }

    // 2. Check player-ammo pickup
    for (auto it = m_level_manager->get_ammo_pickups().begin(); it != m_level_manager->get_ammo_pickups().end(); ++it)
    {
        if (check_collider_collision(m_player_manager->get_player().collider, it->collider))
        {
            m_level_manager->play_ammo_pickup_sound();
            m_player_manager->refill_ammo();
            it = m_level_manager->get_ammo_pickups().erase(it);
            --it;
        }
    }

    // 3. Check bullet-enemies
    for (auto it_bullet = m_player_manager->get_bullets().begin(); it_bullet != m_player_manager->get_bullets().end();
         ++it_bullet)
    {
        for (auto it_enemy = m_enemies_manager->get_enemies().begin(); it_enemy != m_enemies_manager->get_enemies().
             end(); ++it_enemy)
        {
            if (check_collider_collision(it_enemy->collider, it_bullet->collider))
            {
                m_enemies_manager->play_enemy_explode_sound(*it_enemy);
                // Handle bullet delete
                it_bullet = m_player_manager->get_bullets().erase(it_bullet);
                --it_bullet; // as it will be add again in for, so we go back one step
                // Handle enemy delete
                it_enemy = m_enemies_manager->get_enemies().erase(it_enemy);
                --it_enemy;
            }
        }
    }
}

void game_manager::check_game_end()
{
    if (m_enemies_manager->get_current_wave() == m_enemies_manager->get_total_waves() - 1)
    {
        m_game_state = game_state::ending;
    }
}

void game_manager::initialize_managers()
{
    m_level_manager = std::make_shared<level_manager>();
    m_player_manager = std::make_shared<player_manager>();
    m_enemies_manager = std::make_shared<enemies_manager>();
}

void game_manager::initialize_assets()
{
    m_assets_manager = std::make_shared<retro::assets::asset_manager>();
}

void game_manager::initialize_audio()
{
    m_audio_context = std::make_shared<retro::audio::audio_context>();
}

void game_manager::initialize_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f,
                                                       1000.0f);
    m_camera->set_position({24.0f, 0.0f, 25.0f});
}

void game_manager::initialize_fonts()
{
    m_font = std::make_shared<retro::renderer::font>("resources/fonts/arial.ttf");
}

void game_manager::initialize_shaders()
{
#ifdef ASSETS_FROM_PACK
#if (ASSETS_FROM_PACK == 1)
    m_font_shader = m_assets_manager->get_asset_pack(retro::assets::asset_type::shader)->get_asset<
        retro::renderer::shader, retro::assets::asset_type::shader>("font.rrs");
#else
    m_font_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/font.rrs");
#endif
#endif
}

void game_manager::initialize_texts()
{
    m_fps_text = std::make_shared<retro::renderer::text>("FPS: 0", glm::vec2(25.0f,
                                                                             25.0f),
                                                         glm::vec2(0.5f, 0.5f));
    m_enemies_left_text = std::make_shared<retro::renderer::text>("Enemies Left: 0", glm::vec2(25.0f,
                                                                      retro::renderer::renderer::get_viewport_size().y -
                                                                      25.0f),
                                                                  glm::vec2(0.5f, 0.5f));
    m_ammo_text = std::make_shared<retro::renderer::text>("Ammo Left: 0", glm::vec2(
                                                              retro::renderer::renderer::get_viewport_size().x - 175.0f,
                                                              retro::renderer::renderer::get_viewport_size().y -
                                                              25.0f),
                                                          glm::vec2(0.5f, 0.5f));
}

bool game_manager::on_key_pressed(retro::events::key_pressed_event& key_pressed_event)
{
    if (m_game_state == game_state::starting)
    {
        if (key_pressed_event.get_key_code() == retro::key::Enter)
        {
            start_game();
            return true;
        }
    }
    if (m_game_state == game_state::playing)
    {
        if (key_pressed_event.get_key_code() == retro::key::W)
        {
            m_player_manager->move_player_up();
            return true;
        }

        if (key_pressed_event.get_key_code() == retro::key::S)
        {
            m_player_manager->move_player_down();
            return true;
        }
        if (key_pressed_event.get_key_code() == retro::key::Space)
        {
            m_player_manager->player_shoot();
            return true;
        }
        if (key_pressed_event.get_key_code() == retro::key::U)
        {
            save_assets();
            return true;
        }
    }
    return false;
}

bool game_manager::on_window_resize(retro::events::window_resize_event& window_resize_event)
{
    m_enemies_left_text->set_posiiton(glm::vec2(0, window_resize_event.get_size().y - 25.0f));
    m_ammo_text->set_posiiton(glm::vec2(window_resize_event.get_size().x - 175.0f,
                                        window_resize_event.get_size().y -
                                        25.0f));
    return false;
}

void game_manager::save_assets()
{
    m_assets_manager->get_asset_pack(retro::assets::asset_type::shader)->save_asset(
        m_font_shader);
    m_player_manager->save_assets();
    m_level_manager->save_assets();
    m_enemies_manager->save_assets();
    m_assets_manager->serialize_packs();
}

bool game_manager::check_collider_collision(const box_collider& collider1, const box_collider& collider2)
{
    // Calculate the half sizes of the boxes
    glm::vec3 halfSize1 = collider1.size * 0.5f;
    glm::vec3 halfSize2 = collider2.size * 0.5f;

    // Calculate the centers of the boxes
    glm::vec3 center1 = collider1.position + halfSize1;
    glm::vec3 center2 = collider2.position + halfSize2;

    // Calculate the distance between the centers of the boxes
    glm::vec3 distance = glm::abs(center1 - center2);

    // Calculate the combined half sizes of the boxes
    glm::vec3 combinedHalfSize = halfSize1 + halfSize2;

    // Check for overlap along each axis
    bool overlapX = distance.x < combinedHalfSize.x;
    bool overlapY = distance.y < combinedHalfSize.y;
    bool overlapZ = distance.z < combinedHalfSize.z;

    // Return true if there is overlap along all axes
    return overlapX && overlapY && overlapZ;
}
