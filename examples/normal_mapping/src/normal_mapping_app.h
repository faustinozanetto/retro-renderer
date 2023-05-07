#pragma once

#include "retro.h"

class normal_mapping_app : public retro::core::application
{
public:
    normal_mapping_app();
    ~normal_mapping_app() override;

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
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::model> m_model;

    std::shared_ptr<retro::renderer::point_light> m_point_light;
    glm::vec3 m_light_pos;
    glm::vec3 m_light_color;
};
