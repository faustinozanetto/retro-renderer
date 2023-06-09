#pragma once

#include "physics/physics_utils.h"
#include "physics/physics_error_callback.h"
#include "utils/singleton.h"

#include <PxPhysicsAPI.h>
#include <PxPhysics.h>

#include <glm/glm.hpp>

namespace retro::physics
{
	class physics_world : public singleton<physics_world>
	{
	public:
		physics_world(const glm::vec3 &gravity = glm::vec3(0.0f, -9.81f, 0.0f));
		~physics_world();

		/* Getters */
		physx::PxPhysics *get_physics() { return m_physics; }
		physx::PxScene *get_scene() { return m_scene; }
		bool get_is_simulating() const { return m_gravity == glm::vec3(0.0f); }

		const glm::vec3 &get_gravity() const { return m_gravity; }

		/* Setters */
		void set_gravity(const glm::vec3 &gravity);
		void set_is_simulating(bool is_simulating);

		/* Functions */
		void on_update();
		void sync_transforms();

	private:
		void initialize_physx();
		void cleanup();

		/* NVIDIA PhysX Variables */
		physx::PxDefaultAllocator m_allocator;
		physics_error_callback m_error_callback;
		physx::PxPhysics *m_physics;
		physx::PxFoundation *m_foundation;
		physx::PxScene *m_scene;
		physx::PxOmniPvd *m_omni_pvd;
		physx::PxPvd *m_pvd;
		physx::PxDefaultCpuDispatcher *m_dispatcher;

		/* Common Variables */
		glm::vec3 m_gravity;
	};
}