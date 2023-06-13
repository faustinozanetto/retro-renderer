#pragma once

#include "physics/constraints/physics_joint.h"

namespace retro::physics
{
    class physics_distance_joint : public physics_joint
    {
    public:
        physics_distance_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform& t0,
            const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform& t1, float min_distance = 1.0f, float max_distance = 2.0f);
        ~physics_distance_joint() override;

        /* Getters */
        physx::PxDistanceJoint *get_physx_joint() const { return m_physx_distance_joint; }
        float get_min_distance() const { return m_min_distance; }
        float get_max_distance() const { return m_max_distance; }

        /* Setters */
        void set_min_distance(float min_distance);
        void set_max_distance(float max_distance);

    protected:
        void release_joint() override;

    private:
        float m_min_distance;
        float m_max_distance;
        physx::PxDistanceJoint *m_physx_distance_joint;
    };
}