#pragma once

#include "retro.h"

class parallax_mapping_app : public retro::core::application
{
public:
    parallax_mapping_app();
    ~parallax_mapping_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_model();
    void setup_light();
    void setup_camera();

private:
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::renderer::texture> m_albedo_texture;
    std::shared_ptr<retro::renderer::texture> m_normal_texture;
    std::shared_ptr<retro::renderer::texture> m_height_texture;
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::model> m_model;

    glm::vec3 m_object_pos;
    glm::vec3 m_object_rot;

    float m_parallax_scale;

    std::shared_ptr<retro::renderer::point_light> m_point_light;
    glm::vec3 m_light_pos;
    glm::vec3 m_light_color;
};
