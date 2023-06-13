#pragma once

#include "physics/constraints/physics_joint.h"

namespace retro::physics
{
    class physics_distance_joint : public physics_joint
    {
    public:
        physics_distance_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                               const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1, float min_distance = 1.0f, float max_distance = 2.0f);
        ~physics_distance_joint() override;

        /* Getters */
        float get_distance() const;
        float get_min_distance() const;
        float get_max_distance() const;
        float get_tolerance() const;
        float get_stiffness() const;
        float get_damping() const;
        float get_contact_distance() const;
        physx::PxDistanceJointFlags get_distance_joint_flags() const;

        /* Setters */
        void set_min_distance(float distance);
        void set_max_distance(float distance);
        void set_tolerance(float tolerance);
        void set_stiffness(float stiffness);
        void set_damping(float damping);
        void set_contact_distance(float contactDistance);
        void set_distance_joint_flags(physx::PxDistanceJointFlags flags);
        void set_distance_joint_flag(physx::PxDistanceJointFlag::Enum flag, bool value);

    protected:
        void release_joint() override;

    private:
        physx::PxDistanceJoint *m_physx_distance_joint;
    };
}