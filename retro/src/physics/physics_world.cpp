#include "rtpch.h"
#include "physics_world.h"

namespace retro::physics
{
    physics_world::physics_world()
    {
        initialize();
    }

    physics_world::~physics_world()
    {
    }

    void physics_world::initialize()
    {
		// Create the foundation
		m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_error_callback);
        RT_ASSERT_MSG(m_foundation, "An error occurred while creating PhysX Foundation!");

		// Create the physics instance
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale());
        RT_ASSERT_MSG(m_physics, "An error occurred while creating PhysX Instance!");

		// Create the default CPU dispatcher
		m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
        RT_ASSERT_MSG(m_dispatcher, "An error occurred while creating PhysX Dispatcher!");

		// Create the scene
		physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
		sceneDesc.cpuDispatcher = m_dispatcher;
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		m_scene = m_physics->createScene(sceneDesc);
        RT_ASSERT_MSG(m_scene, "An error occurred while creating PhysX Scene!");
    }
}