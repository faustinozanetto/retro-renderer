#pragma once

#include <PxPhysicsAPI.h>
#include <PxPhysics.h>

namespace retro::physics
{
    class physics_world
    {
    public:
		physics_world();
		~physics_world();

		/* Functions */
		void initialize();

		void on_update();

    private:
		physx::PxDefaultAllocator m_allocator;
		physx::PxDefaultErrorCallback m_error_callback;
		physx::PxPhysics* m_physics;
		physx::PxFoundation* m_foundation;
		physx::PxScene* m_scene;
		physx::PxPvd* m_pvd;
		physx::PxDefaultCpuDispatcher* m_dispatcher;
    };
}