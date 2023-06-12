#include "rtpch.h"
#include "physics_fixed_joint.h"

#include "physics/physics_world.h"

namespace retro::physics
{
    physics_fixed_joint::physics_fixed_joint(const std::shared_ptr<physics_actor>& physics_actor_a, physx::PxTransform* t0, const std::shared_ptr<physics_actor>& physics_actor_b, physx::PxTransform* t1) : physics_joint(physics_actor_a, physics_actor_b)
    {
        m_t0 = t0;
        m_t1 = t1;
        initialize_joint();
    }

    physics_fixed_joint::~physics_fixed_joint()
    {
        release_joint();
    }

    void physics_fixed_joint::initialize_joint()
    {
        // Create the fixed joint
        m_physx_fixed_joint = PxFixedJointCreate(*physics_world::get().get_physics(), m_physics_actor_a ? m_physics_actor_a->get_physx_rigid_actor() : nullptr,
			m_t0 ? *m_t0 : physx::PxTransform(physx::PxIDENTITY::PxIdentity), m_physics_actor_b ? m_physics_actor_b->get_physx_rigid_actor() : nullptr, m_t1 ? *m_t1 : physx::PxTransform(physx::PxIDENTITY::PxIdentity));
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