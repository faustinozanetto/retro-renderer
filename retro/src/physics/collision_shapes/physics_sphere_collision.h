#pragma once

#include "physics/collision_shapes/physics_collision_shape.h"

namespace retro::physics
{
    class physics_sphere_collision : public physics_collision_shape
    {
    public:
        physics_sphere_collision(physx::PxPhysics *physics_world, physx::PxMaterial *material, float radius = 1.0f);
        ~physics_sphere_collision() override;

        /* Getters */
        float get_radius() const { return m_radius; }
        physx::PxShape *get_physx_shape() const override;

    private:
        float m_radius;
        physx::PxShape *m_shape;
    };
}