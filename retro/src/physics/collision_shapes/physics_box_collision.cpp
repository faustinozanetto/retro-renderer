#include "rtpch.h"
#include "physics_box_collision.h"

#include "physics/physics_utils.h"

namespace retro::physics
{
    physics_box_collision::physics_box_collision(const std::shared_ptr<retro::physics::physics_material> &physics_material, const glm::vec3 &half_extents) : physics_collision_shape(physics_material)
    {
        m_half_extents = half_extents;
        m_shape = physics_world::get().get_physics()->createShape(physx::PxBoxGeometry(physics_utils::convert_glm_vec3_to_physx(half_extents)),
                                                                  *m_physics_material->get_physx_material());
    }

    physics_box_collision::~physics_box_collision()
    {
        if (m_shape)
        {
            // Release the existing shape
            m_shape->release();
            m_shape = nullptr;
        }
    }

    physx::PxShape *physics_box_collision::get_physx_shape() const
    {
        RT_PROFILE;
        return m_shape;
    }

    void physics_box_collision::set_half_extents(const glm::vec3 &half_extents)
    {
        RT_PROFILE;
        m_half_extents = half_extents;

        // Recreate the PxShape with the new half extents
        if (m_shape)
        {
            // Release the existing shape
            m_shape->release();
            m_shape = nullptr;
        }

        // Recreate the shape using the new half extents
        m_shape = physics_world::get().get_physics()->createShape(physx::PxBoxGeometry(physics_utils::convert_glm_vec3_to_physx(m_half_extents)),
                                                                  *m_physics_material->get_physx_material());
    }
}