#pragma once

#include "physics/constraints/physics_joint.h"
#include "physics/actors/physics_actor.h"

#include <extensions/PxFixedJoint.h>

namespace retro::physics
{
    class physics_fixed_joint : public physics_joint
    {
    public:
        physics_fixed_joint(const std::shared_ptr<physics_actor> &physics_actor_a, physx::PxTransform* t0, const std::shared_ptr<physics_actor> &physics_actor_b, physx::PxTransform* t1);
        ~physics_fixed_joint() override;

        /* Getters */
        physx::PxFixedJoint* get_physx_joint() const { return m_physx_fixed_joint; }

    protected:
		void initialize_joint() override;
		void release_joint() override;

    private:
        physx::PxFixedJoint *m_physx_fixed_joint;
        physx::PxTransform* m_t0;
        physx::PxTransform* m_t1;
    };
}