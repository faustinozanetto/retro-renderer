#pragma once

#include "retro.h"

class simple_geometry_shader_app : public retro::core::application
{
public:
    simple_geometry_shader_app();
    ~simple_geometry_shader_app() override;

    void on_update() override;

    void load_shaders();
    void setup_model();
    void setup_camera();

private:
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::shader> m_normals_shader;
    std::shared_ptr<retro::renderer::model> m_model;
};
