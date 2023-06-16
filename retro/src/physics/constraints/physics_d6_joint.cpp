#include "rtpch.h"
#include "physics_d6_joint.h"

#include "physics/physics_world.h"

namespace retro::physics
{
    physics_d6_joint::physics_d6_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                                       const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1) : physics_joint(physics_actor_a, physics_actor_b)
    {
        // Create the d6 joint
        physx::PxPhysics *physics = physics_world::get().get_physics();
        physx::PxRigidActor *rigid_actor_a = m_physics_actor_a ? m_physics_actor_a->get_physx_rigid_actor() : nullptr;
        physx::PxRigidActor *rigid_actor_b = m_physics_actor_b ? m_physics_actor_b->get_physx_rigid_actor() : nullptr;

        m_physx_d6_joint = physx::PxD6JointCreate(*physics, rigid_actor_a, t0, rigid_actor_b, t1);
        RT_ASSERT_MSG(m_physx_d6_joint, "An error occurred while creating PhysX d6 joint!");
    }

    physics_d6_joint::~physics_d6_joint()
    {
        release_joint();
    }

    void physics_d6_joint::release_joint()
    {
        if (m_physx_d6_joint)
        {
            m_physx_d6_joint->release();
            m_physx_d6_joint = nullptr;
        }
    }

    void physics_d6_joint::set_motion(physx::PxD6Axis::Enum axis, physx::PxD6Motion::Enum type)
    {
        m_physx_d6_joint->setMotion(axis, type);
    }

    physx::PxD6Motion::Enum physics_d6_joint::get_motion(physx::PxD6Axis::Enum axis) const
    {
        return m_physx_d6_joint->getMotion(axis);
    }

    float physics_d6_joint::get_twist_angle() const
    {
        return m_physx_d6_joint->getTwistAngle();
    }

    float physics_d6_joint::get_twist() const
    {
        return m_physx_d6_joint->getTwistAngle();
    }

    float physics_d6_joint::get_swing_y_angle() const
    {
        return m_physx_d6_joint->getSwingYAngle();
    }

    float physics_d6_joint::get_swing_z_angle() const
    {
        return m_physx_d6_joint->getSwingZAngle();
    }

    void physics_d6_joint::set_distance_limit(const physx::PxJointLinearLimit &limit)
    {
        m_physx_d6_joint->setDistanceLimit(limit);
    }

    physx::PxJointLinearLimit physics_d6_joint::get_distance_limit() const
    {
        return m_physx_d6_joint->getDistanceLimit();
    }

    void physics_d6_joint::set_linear_limit(physx::PxD6Axis::Enum axis, float lower_limit, float upper_limit)
    {
        physx::PxJointLinearLimitPair limit_pair = physx::PxJointLinearLimitPair(lower_limit, upper_limit);
        m_physx_d6_joint->setLinearLimit(axis, limit_pair);
    }

    physx::PxJointLinearLimitPair physics_d6_joint::get_linear_limit(physx::PxD6Axis::Enum axis) const
    {
        return m_physx_d6_joint->getLinearLimit(axis);
    }

    void physics_d6_joint::set_twist_limit(float lower_limit, float upper_limit)
    {
        physx::PxJointAngularLimitPair limit_pair = physx::PxJointAngularLimitPair(lower_limit, upper_limit);
        m_physx_d6_joint->setTwistLimit(limit_pair);
    }

    physx::PxJointAngularLimitPair physics_d6_joint::get_twist_limit() const
    {
        return m_physx_d6_joint->getTwistLimit();
    }

    void physics_d6_joint::set_swing_limit(const physx::PxJointLimitCone &limit)
    {
        m_physx_d6_joint->setSwingLimit(limit);
    }

    physx::PxJointLimitCone physics_d6_joint::get_swing_limit() const
    {
        return m_physx_d6_joint->getSwingLimit();
    }

    void physics_d6_joint::set_pyramid_swing_limit(const physx::PxJointLimitPyramid &limit)
    {
        m_physx_d6_joint->setPyramidSwingLimit(limit);
    }

    physx::PxJointLimitPyramid physics_d6_joint::get_pyramid_swing_limit() const
    {
        return m_physx_d6_joint->getPyramidSwingLimit();
    }

    void physics_d6_joint::set_drive(physx::PxD6Drive::Enum index, const physx::PxD6JointDrive &drive)
    {
        m_physx_d6_joint->setDrive(index, drive);
    }

    physx::PxD6JointDrive physics_d6_joint::get_drive(physx::PxD6Drive::Enum index) const
    {
        return m_physx_d6_joint->getDrive(index);
    }

    void physics_d6_joint::set_drive_position(const physx::PxTransform &pose, bool autowake)
    {
        m_physx_d6_joint->setDrivePosition(pose, autowake);
    }

    physx::PxTransform physics_d6_joint::get_drive_position() const
    {
        return m_physx_d6_joint->getDrivePosition();
    }

    void physics_d6_joint::set_drive_velocity(const physx::PxVec3 &linear, const physx::PxVec3 &angular, bool autowake)
    {
        m_physx_d6_joint->setDriveVelocity(linear, angular, autowake);
    }

    void physics_d6_joint::get_drive_velocity(physx::PxVec3 &linear, physx::PxVec3 &angular) const
    {
        m_physx_d6_joint->getDriveVelocity(linear, angular);
    }

    void physics_d6_joint::set_projection_linear_tolerance(float tolerance)
    {
        m_physx_d6_joint->setProjectionLinearTolerance(tolerance);
    }

    float physics_d6_joint::get_projection_linear_tolerance() const
    {
        return m_physx_d6_joint->getProjectionLinearTolerance();
    }

    void physics_d6_joint::set_projection_angular_tolerance(float tolerance)
    {
        m_physx_d6_joint->setProjectionAngularTolerance(tolerance);
    }

    float physics_d6_joint::get_projection_angular_tolerance() const
    {
        return m_physx_d6_joint->getProjectionAngularTolerance();
    }
}