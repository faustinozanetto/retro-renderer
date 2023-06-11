#pragma once

#include <PxPhysicsAPI.h>
#include <PxPhysics.h>

#include <glm/glm.hpp>

namespace retro::physics
{
    class physics_world
    {
    public:
		physics_world();
		~physics_world();

		/* Getters */
		physx::PxPhysics* get_physics() { return m_physics; }
		physx::PxScene* get_scene() { return m_scene; }

		const glm::vec3& get_gravity() const { return m_gravity; }

		/* Setters */
		void set_gravity(const glm::vec3& gravity);

		/* Functions */
		void initialize();
		void cleanup();

		void on_update();

    private:
		/* NVIDIA PhysX Variables */
		physx::PxDefaultAllocator m_allocator;
		physx::PxDefaultErrorCallback m_error_callback;
		physx::PxPhysics* m_physics;
		physx::PxFoundation* m_foundation;
		physx::PxScene* m_scene;
		physx::PxOmniPvd* m_omni_pvd;
		physx::PxDefaultCpuDispatcher* m_dispatcher;

		/* Common Variables */
		glm::vec3 m_gravity;
    };
}