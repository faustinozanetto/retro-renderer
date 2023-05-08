#pragma once

#include "retro.h"

class simple_triangle_textured_app : public retro::core::application
{
public:
    simple_triangle_textured_app();
    ~simple_triangle_textured_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_triangle();

private:
    /* Common Variables */
    std::shared_ptr<retro::renderer::texture> m_texture;
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_triangle_vao;
};
