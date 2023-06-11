#include "rtpch.h"
#include "physics_dynamic_actor.h"

namespace retro::physics
{
    physics_dynamic_actor::physics_dynamic_actor(physx::PxPhysics *physics_world, float mass)
    {
        m_rigid_dynamic = physics_world->createRigidDynamic(pxTransform);

        // Set mass and inertia
        m_mass = mass;
        physx::PxRigidBodyExt::updateMassAndInertia(*m_rigid_dynamic, m_mass);
    }

    physics_dynamic_actor::~physics_dynamic_actor()
    {
    }
}