#pragma once

#include "physics/actors/physics_static_actor.h"

#include "scene/actors/components/base_component.h"

namespace retro::scene
{
    struct physics_joint_component : public base_component
    {
    public:
        virtual ~physics_joint_component() override = default;
    };
}