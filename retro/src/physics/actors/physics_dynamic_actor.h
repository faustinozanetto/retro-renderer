#pragma once

#include "physics/actors/physics_actor.h"

#include <PxPhysicsAPI.h>

namespace retro::physics
{
    class physics_dynamic_actor : public physics_actor
    {
    public:
        physics_dynamic_actor(const std::shared_ptr<physics_world> & world, const glm::vec3 &location = glm::vec3(0.0f), const glm::vec3 &rotation = glm::vec3(0.0f), float mass = 1.0f);
        ~physics_dynamic_actor() override;

        /* Getters */
        physx::PxRigidDynamic *get_physx_rigid_dynamic() const { return m_rigid_dynamic; }
        float get_mass() const { return m_mass; }
		glm::vec3 get_linear_velocity() const;
		glm::vec3 get_angular_velocity() const;
        glm::vec3 get_center_of_mass() const;

        /* Setters */
        void set_mass(float mass);
		void set_linear_velocity(const glm::vec3& linear_velocity);
		void set_angular_velocity(const glm::vec3& angular_velocity);
		void set_center_of_mass(const glm::vec3& center_of_mass);

        /* Functions */
        void initialize() override;

    protected:
        void attach_collision_shapes() override;

    private:
        physx::PxRigidDynamic *m_rigid_dynamic;
        float m_mass;
    };
}