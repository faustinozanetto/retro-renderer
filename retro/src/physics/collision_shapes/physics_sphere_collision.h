#pragma once

#include "physics/collision_shapes/physics_collision_shape.h"

namespace retro::physics
{
    class physics_sphere_collision : public physics_collision_shape
    {
    public:
        physics_sphere_collision(const std::shared_ptr<retro::physics::physics_material> &physics_material, float radius = 1.0f);
        ~physics_sphere_collision() override;

        /* Getters */
        float get_radius() const { return m_radius; }
        physx::PxShape *get_physx_shape() const override;

        /* Setters */
        void set_radius(float radius);

    private:
        float m_radius;
        physx::PxShape *m_shape;
    };
}