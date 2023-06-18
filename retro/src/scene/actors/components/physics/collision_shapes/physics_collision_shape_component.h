#pragma once

#include "scene/actors/components/base_component.h"

namespace retro::scene
{
    class physics_collision_shape_component : public base_component
    {
    public:
        virtual ~physics_collision_shape_component() override = default;
    };
}