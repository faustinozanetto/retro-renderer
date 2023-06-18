#pragma once

#include "physics/constraints/physics_d6_joint.h"

#include "scene/actors/components/physics/joints/physics_joint_component.h"

namespace retro::scene
{
    class physics_d6_joint_component : public physics_joint_component
    {
    public:
        physics_d6_joint_component(const std::shared_ptr<physics::physics_d6_joint> &d6_joint);
        ~physics_d6_joint_component() override;

        /* Getters */
        const std::shared_ptr<physics::physics_d6_joint> &get_d6_joint() const { return m_d6_joint; }

        /* Setters */
        void set_d6_joint(const std::shared_ptr<physics::physics_d6_joint> &d6_joint) { m_d6_joint = d6_joint; }

    private:
        std::shared_ptr<physics::physics_d6_joint> m_d6_joint;
    };
}