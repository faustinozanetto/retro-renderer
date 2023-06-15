#include "rtpch.h"
#include "physics_static_actor.h"

#include "physics/physics_utils.h"

namespace retro::physics
{
    physics_static_actor::physics_static_actor(physx::PxRigidStatic* rigid_static) : physics_actor()
    {
        m_rigid_static = rigid_static;
    }

    physics_static_actor::physics_static_actor(const glm::vec3 &location, const glm::vec3 &rotation) : physics_actor()
    {
        const physx::PxTransform& transform = physics_utils::create_transform_from_glm(location, rotation);
        m_rigid_static = physics_world::get().get_physics()->createRigidStatic(transform);
    }

    physics_static_actor::~physics_static_actor()
    {
        m_rigid_static->release();
        m_rigid_static = nullptr;
    }

    void physics_static_actor::initialize()
    {
        RT_PROFILE;
        // Attach collision shapes.
        attach_collision_shapes();

        // Add actor to physics world
        physics_world::get().get_scene()->addActor(*m_rigid_static);

        m_initialized = true;
    }

    void physics_static_actor::attach_collision_shapes()
    {
        RT_PROFILE;
        for (const auto &collision_shape : m_collision_shapes)
        {
            m_rigid_static->attachShape(*collision_shape->get_physx_shape());
        }
    }
}
