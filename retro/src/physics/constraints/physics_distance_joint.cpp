#include "rtpch.h"
#include "physics_distance_joint.h"

#include "physics/physics_world.h"

namespace retro::physics
{
    physics_distance_joint::physics_distance_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform& t0,
        const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform& t1, float min_distance, float max_distance) : physics_joint(physics_actor_a, physics_actor_b)
    {
        m_min_distance = min_distance;
        m_max_distance = max_distance;

		// Create the fixed joint
		m_physx_distance_joint = PxDistanceJointCreate(*physics_world::get().get_physics(), m_physics_actor_a ? m_physics_actor_a->get_physx_rigid_actor() : nullptr,
			t0, m_physics_actor_b ? m_physics_actor_b->get_physx_rigid_actor() : nullptr, t1);

		set_min_distance(m_min_distance);
		set_max_distance(m_max_distance);
    }

    physics_distance_joint::~physics_distance_joint()
    {
        release_joint();
    }

    void physics_distance_joint::set_min_distance(float min_distance)
    {
        RT_ASSERT_MSG(min_distance < m_max_distance, "Min distance cannot be smaller than max distance!");
        m_min_distance = min_distance;
        m_physx_distance_joint->setMinDistance(m_min_distance);
    }

    void physics_distance_joint::set_max_distance(float max_distance)
    {
        RT_ASSERT_MSG(max_distance > m_min_distance, "Max distance cannotbe smaller than min distance!");
		m_max_distance = max_distance;
		m_physx_distance_joint->setMaxDistance(m_max_distance);
    }

    void physics_distance_joint::release_joint()
    {
        if (m_physx_distance_joint)
        {
            m_physx_distance_joint->release();
            m_physx_distance_joint = nullptr;
        }
    }
}