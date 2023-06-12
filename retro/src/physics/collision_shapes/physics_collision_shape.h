#pragma once

#include "physics/physics_world.h"
#include "physics/physics_material.h"

#include <PxPhysicsAPI.h>

namespace retro::physics
{
    class physics_collision_shape
    {
    public:
        physics_collision_shape(const std::shared_ptr<retro::physics::physics_material>& physics_material) : m_physics_material(physics_material) {}
        virtual ~physics_collision_shape() = default;

        /* Getters */
        const std::shared_ptr<retro::physics::physics_material>& get_physics_material() const { return m_physics_material; }
        virtual physx::PxShape *get_physx_shape() const = 0;

    protected:
        std::shared_ptr<retro::physics::physics_material> m_physics_material;
    };
}