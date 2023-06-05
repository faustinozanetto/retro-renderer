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

    void on_handle_event(retro::events::base_event& event) override;
    bool on_window_resize(retro::events::window_resize_event& resize_event) override;

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;

    /* Model Variables */
    std::shared_ptr<retro::renderer::texture> m_albedo_texture;
    std::shared_ptr<retro::renderer::texture> m_normal_texture;
    std::shared_ptr<retro::renderer::texture> m_height_texture;
    std::shared_ptr<retro::renderer::model> m_model;
    std::shared_ptr<retro::renderer::shader> m_shader;
    glm::vec3 m_object_pos;
    glm::vec3 m_object_rot;

    /* Parallax Variables */
    float m_parallax_scale;

    /* Light Variables */
    std::shared_ptr<retro::renderer::point_light> m_point_light;
};
