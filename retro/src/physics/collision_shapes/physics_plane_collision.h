#pragma once

#include "physics/collision_shapes/physics_collision_shape.h"

namespace retro::physics
{
    class physics_plane_collision : public physics_collision_shape
    {
    public:
        physics_plane_collision(physx::PxPhysics *physics_world, physx::PxMaterial *material);
        ~physics_plane_collision() override;

        /* Getters */
        physx::PxShape *get_physx_shape() const override;

    private:
        physx::PxShape *m_shape;
    };
}