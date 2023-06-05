#pragma once

#include "retro.h"

class ssao_app : public retro::core::application
{
public:
    ssao_app();
    ~ssao_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_model();
    void setup_camera();
    void setup_fbo();
    void setup_screen_quad();
    void setup_ssao();
    void setup_light();

    void on_handle_event(retro::events::base_event& event) override;
    bool on_window_resize(retro::events::window_resize_event& resize_event) override;

private:

    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::renderer::vertex_array_object> m_quad_vao;

    /* Model Variables */
    std::shared_ptr<retro::renderer::texture> m_albedo_texture;
    std::shared_ptr<retro::renderer::texture> m_normal_texture;
    std::shared_ptr<retro::renderer::shader> m_geometry_shader;
    std::shared_ptr<retro::renderer::model> m_model;
    std::shared_ptr<retro::renderer::frame_buffer> m_geometry_fbo;

    /* Light Variables */
    std::shared_ptr<retro::renderer::model> m_light_model;
    std::shared_ptr<retro::renderer::shader> m_lighting_shader;
    glm::vec3 m_light_pos;
    glm::vec3 m_light_color;

    /* SSAO Variables */
    std::shared_ptr<retro::renderer::shader> m_ssao_shader;
    std::shared_ptr<retro::renderer::shader> m_ssao_blur_shader;
    std::vector<glm::vec3> m_ssao_kernel;
    std::shared_ptr<retro::renderer::texture> m_ssao_noise_texture;
    std::shared_ptr<retro::renderer::frame_buffer> m_ssao_color_fbo;
    std::shared_ptr<retro::renderer::frame_buffer> m_ssao_blur_fbo;
    float m_ssao_radius;
    float m_ssao_bias;
    float m_ssao_noise_size;
    bool m_use_ssao;
};
