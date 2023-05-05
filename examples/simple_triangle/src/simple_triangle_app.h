#pragma once

#include "retro.h"

class simple_triangle_app : public retro::core::application
{
public:
    simple_triangle_app();
    ~simple_triangle_app() override;

    void load_shaders();
};
