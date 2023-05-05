#pragma once

#include "retro.h"

class simple_triangle_app : public retro::core::application
{
public:
    simple_triangle_app();
    ~simple_triangle_app() override;

    void load_shaders();
    void setup_triangle();

private:
    std::shared_ptr<retro::renderer::vertex_array_object> m_triangle_vao;
};
