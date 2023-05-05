#pragma once

#include "retro.h"

class simple_triangle_textured_app : public retro::core::application
{
public:
    simple_triangle_textured_app();
    ~simple_triangle_textured_app() override;

    void on_update() override;

    void load_shaders();
    void setup_triangle();

private:
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_triangle_vao;
};
