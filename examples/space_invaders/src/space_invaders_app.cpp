#include "space_invaders_app.h"

#include "core/entry_point.h"

space_invaders_app::space_invaders_app() : application("./")
{
    initialize_managers();
}

space_invaders_app::~space_invaders_app()
{
}

void space_invaders_app::on_update()
{
    m_game_manager->update_game();
}

void space_invaders_app::initialize_managers()
{
    m_game_manager = std::make_shared<game_manager>();
}

void space_invaders_app::on_handle_event(retro::events::base_event& event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::key_pressed_event>(BIND_EVENT_FN(space_invaders_app::on_key_pressed));
    dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(space_invaders_app::on_window_resize));
}

bool space_invaders_app::on_window_resize(retro::events::window_resize_event& window_resize_event)
{
    application::on_window_resize(window_resize_event);
    return m_game_manager->on_window_resize(window_resize_event);
}

bool space_invaders_app::on_key_pressed(retro::events::key_pressed_event& key_pressed_event)
{
    return m_game_manager->on_key_pressed(key_pressed_event);
}

retro::core::application* retro::core::create_application()
{
    return new space_invaders_app();
}
