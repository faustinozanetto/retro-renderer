#include "rtpch.h"
#include "physics_fixed_joint.h"

#include "physics/physics_world.h"

namespace retro::physics
{
    physics_fixed_joint::physics_fixed_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                                             const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1) : physics_joint(physics_actor_a, physics_actor_b)
    {
        // Create the fixed joint
        m_physx_fixed_joint = physx::PxFixedJointCreate(*physics_world::get().get_physics(), m_physics_actor_a ? m_physics_actor_a->get_physx_rigid_actor() : nullptr,
                                                        t0, m_physics_actor_b ? m_physics_actor_b->get_physx_rigid_actor() : nullptr, t1);
        RT_ASSERT_MSG(m_physx_fixed_joint, "An error occurred while creating PhysX fixed joint!");
    }

    physics_fixed_joint::~physics_fixed_joint()
    {
        release_joint();
    }

    void physics_fixed_joint::release_joint()
    {
        if (m_physx_fixed_joint)
        {
            m_physx_fixed_joint->release();
            m_physx_fixed_joint = nullptr;
        }
    }
}