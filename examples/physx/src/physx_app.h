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
	void setup_fbo();
	void setup_screen_quad();

    void on_handle_event(retro::events::base_event &event) override;
    bool on_window_resize(retro::events::window_resize_event &resize_event) override;
    bool on_key_pressed_event(retro::events::key_pressed_event &key_pressed_event);
    bool on_key_released_event(retro::events::key_released_event & key_released_event);
    bool on_mouse_moved_event(retro::events::mouse_moved_event &mouse_moved_event);

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::camera::camera_controller> m_camera_controller;
    std::shared_ptr<retro::scene::scene> m_scene;
    bool m_move_camera;

    std::vector<std::shared_ptr<retro::physics::physics_joint>> m_joints;

    /* Rendering */
    std::shared_ptr<retro::renderer::shader> m_physics_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_cube_vao;
	std::shared_ptr<retro::renderer::shader> m_geometry_shader;
	std::shared_ptr<retro::renderer::shader> m_screen_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_screen_vao;
	std::shared_ptr<retro::renderer::frame_buffer> m_geometry_fbo;
};
