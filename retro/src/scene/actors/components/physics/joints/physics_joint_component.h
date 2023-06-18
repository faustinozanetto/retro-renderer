#pragma once

#include "scene/actors/components/base_component.h"

namespace retro::scene
{
    class physics_joint_component : public base_component
    {
    public:
        virtual ~physics_joint_component() override = default;
    };
}