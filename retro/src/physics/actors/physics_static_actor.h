#pragma once

#include "physics/actors/physics_actor.h"

#include <PxPhysicsAPI.h>

namespace retro::physics
{
    class physics_static_actor : public physics_actor
    {
    public:
        physics_static_actor(const std::shared_ptr<physics_world>& world, physx::PxRigidStatic* rigid_static);
        physics_static_actor(const std::shared_ptr<physics_world> &world, const glm::vec3 &location = glm::vec3(0.0f), const glm::vec3 &rotation = glm::vec3(0.0f));
        ~physics_static_actor() override;

        /* Getters */
        physx::PxRigidStatic *get_physx_rigid_static() const { return m_rigid_static; }

        /* Functions */
        void initialize() override;

    protected:
        void attach_collision_shapes() override;

    private:
        physx::PxRigidStatic *m_rigid_static;
    };
}