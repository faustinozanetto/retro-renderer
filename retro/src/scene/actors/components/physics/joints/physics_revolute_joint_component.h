#pragma once

#include "physics/constraints/physics_revolute_joint.h"

#include "scene/actors/components/physics/joints/physics_joint_component.h"

namespace retro::scene
{
    class physics_revolute_joint_component : public physics_joint_component
    {
    public:
        physics_revolute_joint_component(const std::shared_ptr<physics::physics_revolute_joint> &revolute_joint);
        ~physics_revolute_joint_component() override;
        
        void initialize() override;
        
        /* Getters */
        const std::shared_ptr<physics::physics_revolute_joint> &get_revolute_joint() const { return m_revolute_joint; }

        /* Setters */
        void set_revolute_joint(const std::shared_ptr<physics::physics_revolute_joint> &revolute_joint) { m_revolute_joint = revolute_joint; }
        
    private:
        std::shared_ptr<physics::physics_revolute_joint> m_revolute_joint;
    };
}