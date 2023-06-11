#include "rtpch.h"
#include "physics_dynamic_actor.h"

#include "physics/physics_utils.h"

namespace retro::physics
{
    physics_dynamic_actor::physics_dynamic_actor(const std::shared_ptr<physics_world> & world, const glm::vec3 &location, const glm::vec3 &rotation, float mass) : physics_actor(world)
    {
        m_rigid_dynamic = m_physics_world->get_physics()->createRigidDynamic(physics_utils::create_transform_from_glm(location, rotation));
        m_mass = mass;
    }

    physics_dynamic_actor::~physics_dynamic_actor()
    {
        m_rigid_dynamic->release();
        m_rigid_dynamic = nullptr;
    }

    void physics_dynamic_actor::initialize()
    {
        // Attach collision shapes.
        attach_collision_shapes();

        // Set mass and inertia
        physx::PxRigidBodyExt::updateMassAndInertia(*m_rigid_dynamic, m_mass);

        // Add actor to physics world
        m_physics_world->get_scene()->addActor(*m_rigid_dynamic);

        m_initialized = true;
    }

    void physics_dynamic_actor::attach_collision_shapes()
    {
        for (const auto& collision_shape : m_collision_shapes)
        {
            m_rigid_dynamic->attachShape(*collision_shape->get_physx_shape());
        }
    }
}
