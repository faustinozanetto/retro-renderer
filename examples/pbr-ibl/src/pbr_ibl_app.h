#pragma once

#include "retro.h"

class pbr_ibl_app : public retro::core::application
{
public:
    pbr_ibl_app();
    ~pbr_ibl_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_model();
    void setup_debug_model();
    void setup_camera();
    void setup_fbo();
    void setup_screen_quad();
    void setup_light();
    void setup_environment();
    void setup_enviroment_cube();
    void setup_environment_quad();
    void setup_environment_cubemap();
    void setup_environment_fbo();
    void setup_environment_equirectangular_map();
    void setup_environment_irradiance_map();
    void setup_environment_prefilter_map();
    void setup_environment_brdf_map();

    void render_skybox();

    void on_handle_event(retro::events::base_event& event) override;
    bool on_window_resize(retro::events::window_resize_event& resize_event) override;

private:

    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::renderer::shader> m_screen_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_quad_vao;

    /* Environment */
    std::shared_ptr<retro::renderer::frame_buffer> m_final_fbo;
    std::shared_ptr<retro::renderer::texture> m_environment_hdr_texture;
    std::shared_ptr<retro::renderer::texture> m_environment_cubemap_texture;
    std::shared_ptr<retro::renderer::texture> m_environment_irradiance_texture;
    std::shared_ptr<retro::renderer::texture> m_environment_prefilter_texture;
    uint32_t m_environment_brdf_texture;
    std::shared_ptr<retro::renderer::shader> m_equirectangular_shader;
    std::shared_ptr<retro::renderer::shader> m_irradiance_shader;
    std::shared_ptr<retro::renderer::shader> m_skybox_shader;
    std::shared_ptr<retro::renderer::shader> m_prefilter_shader;
    std::shared_ptr<retro::renderer::shader> m_brdf_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_environment_cube_vao;
    std::shared_ptr<retro::renderer::vertex_array_object> m_environment_quad_vao;
    std::shared_ptr<retro::renderer::frame_buffer> m_environment_capture_fbo;
    std::shared_ptr<retro::renderer::render_buffer> m_environment_capture_rbo;
    glm::mat4 m_capture_projection;
    std::vector<glm::mat4> m_capture_views;
    int m_irradiance_map_size;
    int m_environment_map_size;
    int m_prefilter_map_size;
    int m_brdf_map_size;
    bool m_use_irradiance;

    /* Model Variables */
    std::shared_ptr<retro::renderer::texture> m_albedo_texture;
    std::shared_ptr<retro::renderer::texture> m_normal_texture;
    std::shared_ptr<retro::renderer::texture> m_roughness_texture;
    std::shared_ptr<retro::renderer::texture> m_metallic_texture;
    std::shared_ptr<retro::renderer::texture> m_ao_texture;
    std::shared_ptr<retro::renderer::shader> m_geometry_shader;
    std::shared_ptr<retro::renderer::model> m_model;

    std::shared_ptr<retro::renderer::frame_buffer> m_geometry_fbo;
    std::shared_ptr<retro::renderer::material> m_material;

    glm::vec3 m_object_pos;
    glm::vec3 m_object_rot;

    /* Debug Model */
    std::shared_ptr<retro::renderer::model> m_debug_sphere;
    std::shared_ptr<retro::renderer::material> m_debug_material;
    float m_debug_roughness;
    float m_debug_metallic;

    /* Light Variables */
    std::shared_ptr<retro::renderer::model> m_light_model;
    std::shared_ptr<retro::renderer::shader> m_lighting_shader;
    std::shared_ptr<retro::renderer::point_light> m_point_light;
};
