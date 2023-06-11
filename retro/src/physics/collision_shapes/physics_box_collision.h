#pragma once

#include "physics/physics_collision_shape.h"

#include <glm/glm.hpp>

namespace retro::physics
{
    class physics_box_collision : public physics_collision_shape
    {
    public:
        physics_box_collision(physx::PxPhysics *physics_world, physx::PxMaterial *material, const glm::vec3 &dimensions = glm::vec3(1.0f));
        ~physics_box_collision() override;

        /* Getters */
        const glm::vec3 &get_dimensions() const { return m_dimensions; }
        physx::PxShape *get_physx_shape() const override;

    private:
        glm::vec3 m_dimensions;
        physx::PxShape *m_shape;
    };
}