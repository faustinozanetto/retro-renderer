#include "rtpch.h"
#include "physics_prismatic_joint.h"

#include "physics/physics_world.h"

namespace retro::physics
{
    physics_prismatic_joint::physics_prismatic_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                                                     const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1) : physics_joint(physics_actor_a, physics_actor_b)
    {
        // Create the fixed joint
        physx::PxPhysics *physics = physics_world::get().get_physics();
        physx::PxRigidActor *rigid_actor_a = m_physics_actor_a ? m_physics_actor_a->get_physx_rigid_actor() : nullptr;
        physx::PxRigidActor *rigid_actor_b = m_physics_actor_b ? m_physics_actor_b->get_physx_rigid_actor() : nullptr;

        m_physx_prismatic_joint = physx::PxPrismaticJointCreate(*physics, rigid_actor_a, t0, rigid_actor_b, t1);
        RT_ASSERT_MSG(m_physx_prismatic_joint, "An error occurred while creating PhysX prismatic joint!");
    }

    physics_prismatic_joint::~physics_prismatic_joint()
    {
        release_joint();
    }

    void physics_prismatic_joint::release_joint()
    {
        if (m_physx_prismatic_joint)
        {
            m_physx_prismatic_joint->release();
            m_physx_prismatic_joint = nullptr;
        }
    }

    void physics_prismatic_joint::set_limit(const physx::PxJointLinearLimitPair &limit)
    {
        m_physx_prismatic_joint->setLimit(limit);
    }

    physx::PxJointLinearLimitPair physics_prismatic_joint::get_limit() const
    {
        return m_physx_prismatic_joint->getLimit();
    }

    void physics_prismatic_joint::set_prismatic_joint_flags(physx::PxPrismaticJointFlags flags)
    {
        m_physx_prismatic_joint->setPrismaticJointFlags(flags);
    }

    void physics_prismatic_joint::set_prismatic_joint_flag(physx::PxPrismaticJointFlag::Enum flag, bool value)
    {
        m_physx_prismatic_joint->setPrismaticJointFlag(flag, value);
    }

    physx::PxPrismaticJointFlags physics_prismatic_joint::get_prismatic_joint_flags() const
    {
        return m_physx_prismatic_joint->getPrismaticJointFlags();
    }

    void physics_prismatic_joint::set_projection_linear_tolerance(float tolerance)
    {
        m_physx_prismatic_joint->setProjectionLinearTolerance(tolerance);
    }

    float physics_prismatic_joint::get_projection_linear_tolerance() const
    {
        return m_physx_prismatic_joint->getProjectionLinearTolerance();
    }

    void physics_prismatic_joint::set_projection_angular_tolerance(float tolerance)
    {
        m_physx_prismatic_joint->setProjectionAngularTolerance(tolerance);
    }

    float physics_prismatic_joint::get_projection_angular_tolerance() const
    {
        return m_physx_prismatic_joint->getProjectionAngularTolerance();
    }

    float physics_prismatic_joint::get_position() const
    {
        return m_physx_prismatic_joint->getPosition();
    }

    float physics_prismatic_joint::get_velocity() const
    {
        return m_physx_prismatic_joint->getVelocity();
    }
}