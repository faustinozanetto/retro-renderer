#include "rtpch.h"
#include "physics_world.h"

#include "core/engine_time.h"

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

        // Create the pvd
        m_pvd = PxCreatePvd(*m_foundation);
		RT_ASSERT_MSG(m_pvd, "An error occurred while creating PhysX PVD!");

		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        m_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		// Create the physics instance
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale(), true, m_pvd);
        RT_ASSERT_MSG(m_physics, "An error occurred while creating PhysX Instance!");

		// Create the default CPU dispatcher
		m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
        RT_ASSERT_MSG(m_dispatcher, "An error occurred while creating PhysX Dispatcher!");

		// Create the scene
		physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
		sceneDesc.cpuDispatcher = m_dispatcher;
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		m_scene = m_physics->createScene(sceneDesc);
        RT_ASSERT_MSG(m_scene, "An error occurred while creating PhysX Scene!");

        // Create pvd client
		physx::PxPvdSceneClient* pvd_client = m_scene->getScenePvdClient();
		if (pvd_client)
		{
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
    }

    void physics_world::on_update()
    {
		if (m_scene) {
            m_scene->simulate(core::time::get_delta_time());
            m_scene->fetchResults(true);
		}
    }
}