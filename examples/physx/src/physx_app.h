#pragma once

#include "retro.h"

class physx_app : public retro::core::application
{
public:
    physx_app();
    ~physx_app() override;

    void on_update() override;

	void load_shaders();
	void setup_camera();
	void setup_screen_quad();
	void setup_frame_buffers();

    void on_handle_event(retro::events::base_event &event) override;
    bool on_window_resize(retro::events::window_resize_event &resize_event) override;
    bool on_key_pressed(retro::events::key_pressed_event &key_pressed_event);

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::physics::physics_world> m_physics_world;
    std::shared_ptr<retro::scene::scene> m_scene;

	/* Rendering */
	std::shared_ptr<retro::renderer::frame_buffer> m_geometry_fbo;
	std::shared_ptr<retro::renderer::shader> m_geometry_shader;
	std::shared_ptr<retro::renderer::shader> m_screen_shader;
	std::shared_ptr<retro::renderer::vertex_array_object> m_screen_vao;
};
