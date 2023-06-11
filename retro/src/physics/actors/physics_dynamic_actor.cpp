#include "rtpch.h"
#include "physics_dynamic_actor.h"

#include "physics/physics_utils.h"

namespace retro::physics
{
    physics_dynamic_actor::physics_dynamic_actor(const glm::vec3 &location, const glm::vec3 &rotation, float mass) : physics_actor()
    {
        physx::PxTransform transform = physics_utils::create_transform_from_glm(location, rotation);
        m_rigid_dynamic = physics_world::get().get_physics()->createRigidDynamic(transform);
        m_mass = mass;
    }

    physics_dynamic_actor::~physics_dynamic_actor()
    {
        m_rigid_dynamic->release();
        m_rigid_dynamic = nullptr;
    }

    glm::vec3 physics_dynamic_actor::get_linear_velocity() const
    {
		return physics_utils::convert_physx_vec3_to_glm(m_rigid_dynamic->getLinearVelocity());
    }

    glm::vec3 physics_dynamic_actor::get_angular_velocity() const
    {
        return physics_utils::convert_physx_vec3_to_glm(m_rigid_dynamic->getAngularVelocity());
    }

    glm::vec3 physics_dynamic_actor::get_center_of_mass() const
    {
        return physics_utils::convert_physx_vec3_to_glm(m_rigid_dynamic->getCMassLocalPose().p);
    }

    void physics_dynamic_actor::set_mass(float mass)
    {
		m_mass = mass;
		m_rigid_dynamic->setMass(m_mass);
    }

    void physics_dynamic_actor::set_linear_velocity(const glm::vec3& linear_velocity)
	{
		m_rigid_dynamic->setLinearVelocity(physics_utils::convert_glm_vec3_to_physx(linear_velocity));
    }

    void physics_dynamic_actor::set_angular_velocity(const glm::vec3& angular_velocity)
    {
        m_rigid_dynamic->setAngularVelocity(physics_utils::convert_glm_vec3_to_physx(angular_velocity));
    }

    void physics_dynamic_actor::set_center_of_mass(const glm::vec3& center_of_mass)
    {
		m_rigid_dynamic->setCMassLocalPose(physics_utils::create_transform_from_glm(center_of_mass));
    }

    void physics_dynamic_actor::initialize()
    {
        // Attach collision shapes.
        attach_collision_shapes();

        // Set mass and inertia
        physx::PxRigidBodyExt::updateMassAndInertia(*m_rigid_dynamic, m_mass);

        // Add actor to physics world
        physics_world::get().get_scene()->addActor(*m_rigid_dynamic);

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
