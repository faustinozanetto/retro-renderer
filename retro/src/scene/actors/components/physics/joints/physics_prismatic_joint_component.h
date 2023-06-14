#pragma once

#include "physics/constraints/physics_prismatic_joint.h"

#include "scene/actors/components/physics/joints/physics_joint_component.h"

namespace retro::scene
{
    struct physics_prismatic_joint_component : public physics_joint_component
    {
    public:
        physics_prismatic_joint_component(const std::shared_ptr<physics::physics_prismatic_joint> &prismatic_joint);
        ~physics_prismatic_joint_component() override;

        /* Getters */
        const std::shared_ptr<physics::physics_prismatic_joint> &get_prismatic_joint() const { return m_prismatic_joint; }

        /* Setters */
        void set_prismatic_joint(const std::shared_ptr<physics::physics_prismatic_joint> &prismatic_joint) { m_prismatic_joint = prismatic_joint; }

    private:
        std::shared_ptr<physics::physics_prismatic_joint> m_prismatic_joint;
    };
}