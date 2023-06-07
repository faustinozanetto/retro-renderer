#pragma once

#include "retro.h"

class bloom_app : public retro::core::application
{
public:
    bloom_app();
    ~bloom_app() override;

    void on_update() override;

    void load_shaders();
    void setup_model();
    void setup_camera();
    void setup_screen_quad();
    void setup_frame_buffers();

    void on_handle_event(retro::events::base_event &event) override;
    bool on_window_resize(retro::events::window_resize_event &resize_event) override;

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;

    /* Rendering */
    std::shared_ptr<retro::renderer::frame_buffer> m_geometry_fbo;
    std::shared_ptr<retro::renderer::frame_buffer> m_lighting_fbo;
    std::shared_ptr<retro::renderer::shader> m_geometry_shader;
    std::shared_ptr<retro::renderer::shader> m_screen_shader;
    std::shared_ptr<retro::renderer::shader> m_pbr_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_screen_vao;

    /* Model Variables */
    std::shared_ptr<retro::renderer::material> m_material;
    std::shared_ptr<retro::renderer::model> m_model;
};
