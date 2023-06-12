#pragma once

#include "retro.h"

class physx_app : public retro::core::application
{
public:
    physx_app();
    ~physx_app() override;

    void on_update() override;

    void load_shaders();
    void setup_cube_vao();
    void setup_camera();

    void on_handle_event(retro::events::base_event &event) override;
    bool on_window_resize(retro::events::window_resize_event &resize_event) override;
    bool on_key_pressed(retro::events::key_pressed_event &key_pressed_event);
    bool on_mouse_button_pressed(retro::events::mouse_button_pressed_event &mouse_button_pressed_event);
    bool on_mouse_button_released(retro::events::mouse_button_released_event &mouse_button_released_event);
    bool on_mouse_moved_event(retro::events::mouse_moved_event &mouse_moved_event);

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::camera::camera_controller> m_camera_controller;
    std::shared_ptr<retro::scene::scene> m_scene;
    bool m_move_camera;

    /* Rendering */
    std::shared_ptr<retro::renderer::shader> m_physics_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_cube_vao;
};
