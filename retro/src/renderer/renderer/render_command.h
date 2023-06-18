#pragma once

#include "math/transform/transform.h"
#include "renderer/materials/material.h"
#include "renderer/models/mesh.h"

namespace retro::renderer
{
    struct render_command
    {
        mesh* mesh;
        material* material;
        math::transform* transform;
    };
}
