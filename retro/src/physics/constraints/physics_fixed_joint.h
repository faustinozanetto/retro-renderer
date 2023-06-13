#pragma once

#include "physics/constraints/physics_joint.h"

#include <extensions/PxFixedJoint.h>

namespace retro::physics
{
    class physics_fixed_joint : public physics_joint
    {
    public:
        physics_fixed_joint(physx::PxFixedJoint* fixed_joint);
		physics_fixed_joint(const std::shared_ptr<physics_actor>& physics_actor_a, const physx::PxTransform& t0,
			const std::shared_ptr<physics_actor>& physics_actor_b, const physx::PxTransform& t1);
        ~physics_fixed_joint() override;

        /* Getters */
        physx::PxFixedJoint* get_physx_joint() const { return m_physx_fixed_joint; }

    protected:
		void release_joint() override;

    private:
        physx::PxFixedJoint *m_physx_fixed_joint;
    };
}