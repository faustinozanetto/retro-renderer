#pragma once

#include "retro.h"

class point_shadows_app : public retro::core::application
{
public:
    point_shadows_app();
    ~point_shadows_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_model();
    void setup_light();
    void setup_light_cube();
    void setup_camera();
    void setup_shadow_map_frame_buffer();

    void render_elements(const std::shared_ptr<retro::renderer::shader> &shader);
    
    void on_handle_event(retro::events::base_event& event) override;
    bool on_window_resize(retro::events::window_resize_event& resize_event) override;

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;

    /* Model Variables */
    std::shared_ptr<retro::renderer::texture> m_texture;
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::model> m_model;
    glm::vec3 m_object_pos;
    glm::vec3 m_object_rot;

    /* Light Variables */
    std::shared_ptr<retro::renderer::point_light> m_point_light;
    std::shared_ptr<retro::renderer::vertex_array_object> m_light_cube_vao;

    /* Shadowss Variables */
    std::shared_ptr<retro::renderer::shader> m_shadow_shader;
    int m_shadow_map_size;
    uint32_t m_shadow_map_fbo;
    uint32_t m_depth_cubemap;

    float m_shadow_bias;
    float m_near_plane;
    float m_far_plane;
};
