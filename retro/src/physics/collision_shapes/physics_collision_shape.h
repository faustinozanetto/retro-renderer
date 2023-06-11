#pragma once

#include <PxPhysicsAPI.h>

namespace retro::physics
{
    class physics_collision_shape
    {
    public:
        virtual ~physics_collision_shape() = default;

        /* Getters */
        virtual physx::PxShape *get_physx_shape() const = 0;
    };
}