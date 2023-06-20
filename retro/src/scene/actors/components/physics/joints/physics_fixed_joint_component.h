#pragma once

#include "physics/constraints/physics_fixed_joint.h"

#include "scene/actors/components/physics/joints/physics_joint_component.h"

namespace retro::scene
{
    class physics_fixed_joint_component : public physics_joint_component
    {
    public:
        physics_fixed_joint_component(const std::shared_ptr<physics::physics_fixed_joint> &fixed_joint);
        ~physics_fixed_joint_component() override;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<physics::physics_fixed_joint> &get_fixed_joint() const { return m_fixed_joint; }

        /* Setters */
        void set_fixed_joint(const std::shared_ptr<physics::physics_fixed_joint> &fixed_joint) { m_fixed_joint = fixed_joint; }
        
    private:
        std::shared_ptr<physics::physics_fixed_joint> m_fixed_joint;
    };
}