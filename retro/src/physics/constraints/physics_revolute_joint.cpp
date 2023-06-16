#include "physics_revolute_joint.h"
#include "physics_revolute_joint.h"
#include "rtpch.h"
#include "physics_revolute_joint.h"

#include "physics/physics_world.h"

namespace retro::physics
{
    physics_revolute_joint::physics_revolute_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                                                   const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1) : physics_joint(physics_actor_a, physics_actor_b)
    {
        // Create the revolute joint
        physx::PxPhysics *physics = physics_world::get().get_physics();
        physx::PxRigidActor *rigid_actor_a = m_physics_actor_a ? m_physics_actor_a->get_physx_rigid_actor() : nullptr;
        physx::PxRigidActor *rigid_actor_b = m_physics_actor_b ? m_physics_actor_b->get_physx_rigid_actor() : nullptr;

        m_physx_revolute_joint = physx::PxRevoluteJointCreate(*physics, rigid_actor_a, t0, rigid_actor_b, t1);
        RT_ASSERT_MSG(m_physx_revolute_joint, "An error occurred while creating PhysX revolute joint!");

        m_physx_revolute_joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, true);
        m_physx_revolute_joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);

        m_physx_revolute_joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_FREESPIN, false);

		physx::PxJointAngularLimitPair limit(-physx::PxPi, physx::PxPi);
        m_physx_revolute_joint->setLimit(limit);
    }

    physics_revolute_joint::~physics_revolute_joint()
    {
        release_joint();
    }

    void physics_revolute_joint::release_joint()
    {
        RT_PROFILE;
        if (m_physx_revolute_joint)
        {
            m_physx_revolute_joint->release();
            m_physx_revolute_joint = nullptr;
        }
    }

    float physics_revolute_joint::get_angle() const
    {
        RT_PROFILE;
        return m_physx_revolute_joint->getAngle();
    }

    float physics_revolute_joint::get_velocity() const
    {
        RT_PROFILE;
        return m_physx_revolute_joint->getVelocity();
    }

    void physics_revolute_joint::set_limit(float lower, float upper)
    {
        RT_PROFILE;
        physx::PxJointAngularLimitPair limit(lower, upper);
        m_physx_revolute_joint->setLimit(limit);
    }

    physx::PxJointAngularLimitPair physics_revolute_joint::get_limit() const
    {
        RT_PROFILE;
        return m_physx_revolute_joint->getLimit();
    }

    void physics_revolute_joint::set_drive_velocity(float velocity, bool autowake)
    {
        RT_PROFILE;
        m_physx_revolute_joint->setDriveVelocity(velocity, autowake);
    }

    float physics_revolute_joint::get_drive_velocity() const
    {
        RT_PROFILE;
        return m_physx_revolute_joint->getDriveVelocity();
    }

    void physics_revolute_joint::set_drive_force_limit(float force_limit)
    {
        RT_PROFILE;
        m_physx_revolute_joint->setDriveForceLimit(force_limit);
    }

    float physics_revolute_joint::get_drive_force_limit() const
    {
        RT_PROFILE;
        return m_physx_revolute_joint->getDriveForceLimit();
    }

    void physics_revolute_joint::set_drive_gear_ratio(float gear_ratio)
    {
        RT_PROFILE;
        m_physx_revolute_joint->setDriveGearRatio(gear_ratio);
    }

    float physics_revolute_joint::get_drive_gear_ratio() const
    {
        RT_PROFILE;
        return m_physx_revolute_joint->getDriveGearRatio();
    }

    void physics_revolute_joint::set_joint_flags(physx::PxRevoluteJointFlags flags)
    {
        RT_PROFILE;
        m_physx_revolute_joint->setRevoluteJointFlags(flags);
    }

    void physics_revolute_joint::set_joint_flag(physx::PxRevoluteJointFlag::Enum flag, bool value)
    {
        RT_PROFILE;
        m_physx_revolute_joint->setRevoluteJointFlag(flag, value);
    }

    physx::PxRevoluteJointFlags physics_revolute_joint::get_joint_flags() const
    {
        RT_PROFILE;
        return m_physx_revolute_joint->getRevoluteJointFlags();
    }

    void physics_revolute_joint::set_projection_linear_tolerance(float distance)
    {
        RT_PROFILE;
        m_physx_revolute_joint->setProjectionLinearTolerance(distance);
    }

    float physics_revolute_joint::get_projection_linear_tolerance() const
    {
        RT_PROFILE;
        return m_physx_revolute_joint->getProjectionLinearTolerance();
    }

    void physics_revolute_joint::set_projection_angular_tolerance(float tolerance)
    {
        RT_PROFILE;
        m_physx_revolute_joint->setProjectionAngularTolerance(tolerance);
    }

    float physics_revolute_joint::get_projection_angular_tolerance() const
    {
        RT_PROFILE;
        return m_physx_revolute_joint->getProjectionAngularTolerance();
    }
}