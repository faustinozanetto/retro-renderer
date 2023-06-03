#pragma once

#include "game_manager.h"

#include <retro.h>

class space_invaders_app : public retro::core::application
{
public:
    space_invaders_app();
    ~space_invaders_app() override;

    void on_update() override;

    void initialize_managers();
    
    void on_handle_event(retro::events::base_event& event) override;
    bool on_window_resize(retro::events::window_resize_event& window_resize_event) override;
    bool on_key_pressed(retro::events::key_pressed_event& key_pressed_event);
private:
    /* Game */
    std::shared_ptr<game_manager> m_game_manager;
};
