#include "rtpch.h"
#include "physics_spherical_joint.h"

#include "physics/physics_world.h"

namespace retro::physics
{
    physics_spherical_joint::physics_spherical_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                                                     const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1) : physics_joint(physics_actor_a, physics_actor_b)
    {
        // Create the spherical joint
        physx::PxPhysics *physics = physics_world::get().get_physics();
        physx::PxRigidActor *rigid_actor_a = m_physics_actor_a ? m_physics_actor_a->get_physx_rigid_actor() : nullptr;
        physx::PxRigidActor *rigid_actor_b = m_physics_actor_b ? m_physics_actor_b->get_physx_rigid_actor() : nullptr;

        m_physx_spherical_joint = physx::PxSphericalJointCreate(*physics, rigid_actor_a, t0, rigid_actor_b, t1);
        RT_ASSERT_MSG(m_physx_spherical_joint, "An error occurred while creating PhysX spherical joint!");
    }

    physics_spherical_joint::~physics_spherical_joint()
    {
        release_joint();
    }

    void physics_spherical_joint::release_joint()
    {
        if (m_physx_spherical_joint)
        {
            m_physx_spherical_joint->release();
            m_physx_spherical_joint = nullptr;
        }
    }

    physx::PxJointLimitCone physics_spherical_joint::get_limit_cone() const
    {
        return m_physx_spherical_joint->getLimitCone();
    }

    void physics_spherical_joint::set_limit_cone(const physx::PxJointLimitCone &limit)
    {
        m_physx_spherical_joint->setLimitCone(limit);
    }

    float physics_spherical_joint::get_swing_y_angle() const
    {
        return m_physx_spherical_joint->getSwingYAngle();
    }

    float physics_spherical_joint::get_swing_z_angle() const
    {
        return m_physx_spherical_joint->getSwingZAngle();
    }

    void physics_spherical_joint::set_spherical_joint_flags(physx::PxSphericalJointFlags flags)
    {
        m_physx_spherical_joint->setSphericalJointFlags(flags);
    }

    void physics_spherical_joint::set_spherical_joint_flag(physx::PxSphericalJointFlag::Enum flag, bool value)
    {
        m_physx_spherical_joint->setSphericalJointFlag(flag, value);
    }

    physx::PxSphericalJointFlags physics_spherical_joint::get_spherical_joint_flags() const
    {
        return m_physx_spherical_joint->getSphericalJointFlags();
    }

    void physics_spherical_joint::set_projection_linear_tolerance(float tolerance)
    {
        m_physx_spherical_joint->setProjectionLinearTolerance(tolerance);
    }

    float physics_spherical_joint::get_projection_linear_tolerance() const
    {
        return m_physx_spherical_joint->getProjectionLinearTolerance();
    }
}