#pragma once

#include "retro.h"

class simple_3d_app : public retro::core::application
{
public:
    simple_3d_app();
    ~simple_3d_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_cube();

private:
    std::shared_ptr<retro::renderer::texture> m_texture;
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_cube_vao;
};
