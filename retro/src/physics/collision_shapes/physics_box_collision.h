#pragma once

#include "physics/collision_shapes/physics_collision_shape.h"

#include <glm/glm.hpp>

namespace retro::physics
{
    class physics_box_collision : public physics_collision_shape
    {
    public:
        physics_box_collision(physx::PxMaterial *material, const glm::vec3 & half_extents = glm::vec3(1.0f));
        ~physics_box_collision() override;

        /* Getters */
        const glm::vec3 &get_half_extents() const { return m_half_extents; }
        physx::PxShape *get_physx_shape() const override;

    private:
        glm::vec3 m_half_extents;
        physx::PxShape *m_shape;
    };
}