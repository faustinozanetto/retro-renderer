#include "rtpch.h"
#include "physics_world.h"

#include "core/engine_time.h"
#include "scene/scene_manager.h"
#include "scene/actors/components/transform_component.h"
#include "scene/actors/components/physics/physics_dynamic_actor_component.h"

#include <omnipvd/PxOmniPvd.h>
#include <OmniPvdLoader.h>
#include <OmniPvdWriter.h>
#include <OmniPvdFileWriteStream.h>

#include <gpu/PxGpu.h>
#include "physics_error_callback.h"
#include "scene/actors/components/physics/physics_static_actor_component.h"

namespace retro::physics
{
    physics_world::physics_world(const glm::vec3& gravity)
    {
        m_gravity = gravity;
        initialize_physx();
    }

    physics_world::~physics_world()
    {
        cleanup();
    }

    void physics_world::set_gravity(const glm::vec3& gravity)
    {
        RT_PROFILE;
        m_gravity = gravity;
        m_scene->setGravity(physics_utils::convert_glm_vec3_to_physx(gravity));
    }

    void physics_world::set_is_simulating(bool is_simulating)
    {
        set_gravity(glm::vec3(0.0f));
    }

    void physics_world::initialize_physx()
    {
        RT_PROFILE;
        RT_SEPARATOR();

        RT_TRACE("Retro Renderer | Started initializing PhysX World.");
        // Create the foundation
        m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_error_callback);
        RT_ASSERT_MSG(m_foundation, "An error occurred while creating PhysX Foundation!");

#ifdef RT_DEBUG
        // Create the OmniPVD
        m_omni_pvd = PxCreateOmniPvd(*m_foundation);
        RT_ASSERT_MSG(m_omni_pvd, "An error occurred while creating PhysX Omni PVD!");

        OmniPvdWriter* omni_writer = m_omni_pvd->getWriter();
        RT_ASSERT_MSG(omni_writer, "An error occurred while creating PhysX Omni Writer!");

        OmniPvdFileWriteStream* omni_file_write_stream = m_omni_pvd->getFileWriteStream();
        RT_ASSERT_MSG(omni_file_write_stream, "An error occurred while creating PhysX Omni File Writer!");
        omni_writer->setWriteStream(omni_file_write_stream);

        m_pvd = PxCreatePvd(*m_foundation);
        RT_ASSERT_MSG(m_pvd, "An error occurred while creating PhysX PVD!");

        physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        m_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif
        // Create the physics instance
        m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale(), true, m_pvd,
                                    m_omni_pvd);
        RT_ASSERT_MSG(m_physics, "An error occurred while creating PhysX Instance!");

        PxInitExtensions(*m_physics, m_pvd);
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
        RT_PROFILE;
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
        RT_PROFILE;
        if (m_scene)
        {
            m_scene->simulate(core::time::get_delta_time());
            m_scene->fetchResults(true);
        }
    }

    void physics_world::sync_transforms()
    {
        RT_PROFILE;
        const auto& registry = scene::scene_manager::get().get_active_scene()->get_actors_registry();
        const auto& scene_view = registry->view<scene::transform_component, scene::physics_dynamic_actor_component>();
        for (auto&& [actor, transform_comp, physics_dynamic_actor_comp] : scene_view.each())
        {
            const std::shared_ptr<math::transform>& transform = transform_comp.get_transform();

            if (physics_dynamic_actor_comp.get_dynamic_actor()->get_is_kinematic())
            {
                physx::PxTransform physx_transform = physics_utils::create_transform_from_glm(
                    transform->get_location(), transform->get_rotation());
                physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->setKinematicTarget(
                    physx_transform);
            }
            else
            {
                transform->set_location(physics_utils::convert_physx_vec3_to_glm(physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->getGlobalPose().p));
                transform->set_rotation(physics_utils::convert_physx_quat_to_glm(physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->getGlobalPose().q));
            }
        }
    }
}
