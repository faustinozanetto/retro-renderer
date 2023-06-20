#include "rtpch.h"
#include "physics_material_component.h"

namespace retro::scene
{
    physics_material_component::physics_material_component()
    {
    }

    physics_material_component::physics_material_component(
        const std::shared_ptr<physics::physics_material>& physics_material)
    {
        m_physics_material = physics_material;
    }

    physics_material_component::~physics_material_component()
    {
    }

    void physics_material_component::initialize()
    {
        if (!m_physics_material)
            m_physics_material = std::make_shared<physics::physics_material>();
    }
}
