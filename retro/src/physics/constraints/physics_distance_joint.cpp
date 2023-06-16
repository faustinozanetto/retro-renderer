#include "rtpch.h"
#include "physics_distance_joint.h"

#include "physics/physics_world.h"

namespace retro::physics
{
    physics_distance_joint::physics_distance_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                                                   const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1, float min_distance, float max_distance) : physics_joint(physics_actor_a, physics_actor_b)
    {
        physx::PxPhysics *physics = physics_world::get().get_physics();
        physx::PxRigidActor *rigid_actor_a = m_physics_actor_a ? m_physics_actor_a->get_physx_rigid_actor() : nullptr;
        physx::PxRigidActor *rigid_actor_b = m_physics_actor_b ? m_physics_actor_b->get_physx_rigid_actor() : nullptr;

        m_physx_distance_joint = physx::PxDistanceJointCreate(*physics, rigid_actor_a, t0, rigid_actor_b, t1);
    }

    physics_distance_joint::~physics_distance_joint()
    {
        release_joint();
    }

    void physics_distance_joint::release_joint()
    {
        RT_PROFILE;
        if (m_physx_distance_joint)
        {
            m_physx_distance_joint->release();
            m_physx_distance_joint = nullptr;
        }
    }

    float physics_distance_joint::get_distance() const
    {
        RT_PROFILE;
        return m_physx_distance_joint->getDistance();
    }

    void physics_distance_joint::set_min_distance(float distance)
    {
        RT_PROFILE;
        m_physx_distance_joint->setMinDistance(distance);
    }

    float physics_distance_joint::get_min_distance() const
    {
        RT_PROFILE;
        return m_physx_distance_joint->getMinDistance();
    }

    void physics_distance_joint::set_max_distance(float distance)
    {
        RT_PROFILE;
        m_physx_distance_joint->setMaxDistance(distance);
    }

    float physics_distance_joint::get_max_distance() const
    {
        RT_PROFILE;
        return m_physx_distance_joint->getMaxDistance();
    }

    void physics_distance_joint::set_tolerance(float tolerance)
    {
        RT_PROFILE;
        m_physx_distance_joint->setTolerance(tolerance);
    }

    float physics_distance_joint::get_tolerance() const
    {
        RT_PROFILE;
        return m_physx_distance_joint->getTolerance();
    }

    void physics_distance_joint::set_stiffness(float stiffness)
    {
        RT_PROFILE;
        m_physx_distance_joint->setStiffness(stiffness);
    }

    float physics_distance_joint::get_stiffness() const
    {
        RT_PROFILE;
        return m_physx_distance_joint->getStiffness();
    }

    void physics_distance_joint::set_damping(float damping)
    {
        RT_PROFILE;
        m_physx_distance_joint->setDamping(damping);
    }

    float physics_distance_joint::get_damping() const
    {
        RT_PROFILE;
        return m_physx_distance_joint->getDamping();
    }

    void physics_distance_joint::set_contact_distance(float contactDistance)
    {
        RT_PROFILE;
        m_physx_distance_joint->setContactDistance(contactDistance);
    }

    float physics_distance_joint::get_contact_distance() const
    {
        RT_PROFILE;
        return m_physx_distance_joint->getContactDistance();
    }

    void physics_distance_joint::set_distance_joint_flags(physx::PxDistanceJointFlags flags)
    {
        RT_PROFILE;
        m_physx_distance_joint->setDistanceJointFlags(flags);
    }

    void physics_distance_joint::set_distance_joint_flag(physx::PxDistanceJointFlag::Enum flag, bool value)
    {
        RT_PROFILE;
        m_physx_distance_joint->setDistanceJointFlag(flag, value);
    }

    physx::PxDistanceJointFlags physics_distance_joint::get_distance_joint_flags() const
    {
        RT_PROFILE;
        return m_physx_distance_joint->getDistanceJointFlags();
    }
}