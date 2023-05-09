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
    void setup_camera();
    void setup_fbo();
    void setup_screen_quad();
    void setup_light();
    void setup_environment();
    void setup_enviroment_cube();
    void setup_environment_cubemap();
    void setup_environment_fbo();
    void setup_environment_equirectangular_map();
    void setup_environment_irradiance_map();

    void render_skybox();

    void on_handle_event(retro::events::base_event &event) override;

private:
    bool on_resize_ssao(retro::events::window_resize_event &resize_event);

    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::renderer::shader> m_screen_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_quad_vao;

    /* Environment */
    std::shared_ptr<retro::renderer::frame_buffer> m_final_fbo;
    std::shared_ptr<retro::renderer::texture> m_environment_cubemap_texture;
    std::shared_ptr<retro::renderer::shader> m_equirectangular_shader;
    std::shared_ptr<retro::renderer::shader> m_irradiance_shader;
    std::shared_ptr<retro::renderer::shader> m_skybox_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_environment_cube_vao;
    unsigned int captureFBO, captureRBO;
    unsigned int envCubemap;
    unsigned int irradianceMap;
    glm::mat4 captureProjection;
    std::vector<glm::mat4> captureViews;

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

    /* Light Variables */
    std::shared_ptr<retro::renderer::model> m_light_model;
    std::shared_ptr<retro::renderer::shader> m_lighting_shader;
    glm::vec3 m_light_pos;
    glm::vec3 m_light_color;
};
