#pragma once

#include "physics/constraints/physics_distance_joint.h"

#include "scene/actors/components/physics/joints/physics_joint_component.h"

namespace retro::scene
{
    struct physics_distance_joint_component : public physics_joint_component
    {
    public:
        physics_distance_joint_component(const std::shared_ptr<physics::physics_distance_joint> &distance_joint);
        ~physics_distance_joint_component() override;

        /* Getters */
        const std::shared_ptr<physics::physics_distance_joint> &get_distance_joint() const { return m_distance_joint; }

        /* Setters */
        void set_distance_joint(const std::shared_ptr<physics::physics_distance_joint> &distance_joint) { m_distance_joint = distance_joint; }

    private:
        std::shared_ptr<physics::physics_distance_joint> m_distance_joint;
    };
}