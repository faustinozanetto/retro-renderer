#pragma once

#include "physics/collision_shapes/physics_collision_shape.h"

namespace retro::physics
{
    class physics_capsule_collision : public physics_collision_shape
    {
    public:
        physics_capsule_collision(const std::shared_ptr<retro::physics::physics_material>& physics_material, float radius = 1.0f, float half_height = 0.5f);
        ~physics_capsule_collision() override;

        /* Getters */
        float get_radius() const { return m_radius; }
        float get_half_height() const { return m_half_height; }
        physx::PxShape *get_physx_shape() const override;

    private:
        float m_radius;
        float m_half_height;
        physx::PxShape *m_shape;
    };
}