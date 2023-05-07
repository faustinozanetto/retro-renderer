#pragma once

#include "retro.h"

class simple_lighting_app : public retro::core::application
{
public:
    simple_lighting_app();
    ~simple_lighting_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_model();
    void setup_light();
    void setup_light_cube();
    void setup_camera();

private:
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::renderer::texture> m_texture;
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::shader> m_shader_light;
    std::shared_ptr<retro::renderer::model> m_model;
    std::shared_ptr<retro::renderer::point_light> m_point_light;
    std::shared_ptr<retro::renderer::spot_light> m_spot_light;
    std::shared_ptr<retro::renderer::vertex_array_object> m_light_cube_vao;

    retro::renderer::light_type m_selected_light;
    glm::vec3 m_light_pos;
    glm::vec3 m_light_color;
    glm::vec3 m_light_dir;
    float m_inner_angle;
    float m_outer_angle;
};
