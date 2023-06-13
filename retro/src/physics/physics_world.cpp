#include "rtpch.h"
#include "physics_world.h"

#include "core/engine_time.h"

#include <omnipvd/PxOmniPvd.h>
#include <OmniPvdLoader.h>
#include <OmniPvdWriter.h>
#include <OmniPvdFileWriteStream.h>

#include <gpu/PxGpu.h>
#include "physics_error_callback.h"

namespace retro::physics
{
	physics_world::physics_world(const glm::vec3 &gravity)
	{
		m_gravity = gravity;
		initialize_physx();
	}

	physics_world::~physics_world()
	{
		cleanup();
	}

	void physics_world::set_gravity(const glm::vec3 &gravity)
	{
		m_gravity = gravity;
		m_scene->setGravity(physics_utils::convert_glm_vec3_to_physx(gravity));
	}

	void physics_world::initialize_physx()
	{
		RT_SEPARATOR();

		RT_TRACE("Retro Renderer | Started initializing PhysX World.");
		// Create the foundation
		m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_error_callback);
		RT_ASSERT_MSG(m_foundation, "An error occurred while creating PhysX Foundation!");

		// Create the OmniPVD
#ifdef RT_DEBUG
		m_omni_pvd = PxCreateOmniPvd(*m_foundation);
		RT_ASSERT_MSG(m_omni_pvd, "An error occurred while creating PhysX Omni PVD!");

		OmniPvdWriter *omni_writer = m_omni_pvd->getWriter();
		RT_ASSERT_MSG(omni_writer, "An error occurred while creating PhysX Omni Writer!");

		OmniPvdFileWriteStream *omni_file_write_stream = m_omni_pvd->getFileWriteStream();
		RT_ASSERT_MSG(omni_file_write_stream, "An error occurred while creating PhysX Omni File Writer!");
		omni_writer->setWriteStream(omni_file_write_stream);

		m_pvd = PxCreatePvd(*m_foundation);
		RT_ASSERT_MSG(m_pvd, "An error occurred while creating PhysX PVD!");

		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		m_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif

		// Create the physics instance
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale(), true, m_pvd, m_omni_pvd);
		RT_ASSERT_MSG(m_physics, "An error occurred while creating PhysX Instance!");

		PxInitExtensions(*m_physics, m_pvd);

		/*
		// Enable GPU acceleration
		physx::PxCudaContextManagerDesc cudaDesc;
		cudaDesc.interopMode = physx::PxCudaInteropMode::OGL_INTEROP;  // Specify the desired interop mode
		physx::PxCudaContextManager* cudaContextManager = PxCreateCudaContextManager(*m_foundation, cudaDesc, nullptr);
		if (cudaContextManager)
		{
			if (!cudaContextManager->contextIsValid())
			{
				cudaContextManager->release();
				cudaContextManager = nullptr;
			}
		}
		*/
#ifdef RT_DEBUG
		omni_file_write_stream->setFileName("myoutpufile.ovd");
		m_omni_pvd->startSampling();
#endif

		// Create the default CPU dispatcher
		m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		RT_ASSERT_MSG(m_dispatcher, "An error occurred while creating PhysX Dispatcher!");

		// Create the scene
		physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
		sceneDesc.cpuDispatcher = m_dispatcher;
		//sceneDesc.cudaContextManager = cudaContextManager;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		sceneDesc.gravity = physics_utils::convert_glm_vec3_to_physx(m_gravity);
		m_scene = m_physics->createScene(sceneDesc);
		RT_ASSERT_MSG(m_scene, "An error occurred while creating PhysX Scene!");

		m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
		m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

#ifdef RT_DEBUG
		physx::PxPvdSceneClient* pvd_client = m_scene->getScenePvdClient();
		if (pvd_client)
		{
			pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
#endif
		RT_TRACE("Retro Renderer | PhysX world initialized successfully!");
		RT_SEPARATOR();
	}

	void physics_world::cleanup()
	{
		if (m_scene)
		{
			m_scene->release();
			m_scene = nullptr;
		}
		if (m_dispatcher)
		{
			m_dispatcher->release();
			m_dispatcher = nullptr;
		}
		PxCloseExtensions();
		if (m_physics)
		{
			m_physics->release();
			m_physics = nullptr;
		}
		if (m_omni_pvd)
		{
			m_omni_pvd->release();
			m_omni_pvd = nullptr;
		}
		if (m_foundation)
		{
			m_foundation->release();
			m_foundation = nullptr;
		}
	}

	void physics_world::on_update()
	{
		if (m_scene)
		{
			m_scene->simulate(core::time::get_delta_time());
			m_scene->fetchResults(true);
		}
	}

	void physics_world::sync_transforms()
	{
		
	}
}