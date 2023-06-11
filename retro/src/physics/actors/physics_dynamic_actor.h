#pragma once

#include "physics/actors/physics_actor.h"

#include <PxPhysicsAPI.h>

namespace retro::physics
{
    class physics_dynamic_actor : public physics_actor
    {
    public:
        physics_dynamic_actor(physx::PxPhysics *physics_world, float mass = 1.0f);
        ~physics_dynamic_actor() override;

        /* Getters */
        physx::PxRigidDynamic *get_physx_rigid_dynamic() const { return m_rigid_dynamic; }
        float get_mass() const { return m_mass; }

    private:
        physx::PxRigidDynamic *m_rigid_dynamic;
        float m_mass;
    };
}