#include "rtpch.h"
#include "physics_static_actor.h"

#include "physics/physics_utils.h"

namespace retro::physics
{
    physics_static_actor::physics_static_actor(const std::shared_ptr<physics_world>& world, physx::PxRigidStatic* rigid_static) : physics_actor(world)
    {
        m_rigid_static = rigid_static;
    }

    physics_static_actor::physics_static_actor(const std::shared_ptr<physics_world> &world, const glm::vec3 &location, const glm::vec3 &rotation) : physics_actor(world)
    {
        const physx::PxTransform& transform = physics_utils::create_transform_from_glm(location, rotation);
        m_rigid_static = m_physics_world->get_physics()->createRigidStatic(transform);
    }

    physics_static_actor::~physics_static_actor()
    {
        m_rigid_static->release();
        m_rigid_static = nullptr;
    }

    void physics_static_actor::initialize()
    {
        // Attach collision shapes.
        attach_collision_shapes();

        // Add actor to physics world
        m_physics_world->get_scene()->addActor(*m_rigid_static);

        m_initialized = true;
    }

    void physics_static_actor::attach_collision_shapes()
    {
        for (const auto &collision_shape : m_collision_shapes)
        {
            m_rigid_static->attachShape(*collision_shape->get_physx_shape());
        }
    }
}
