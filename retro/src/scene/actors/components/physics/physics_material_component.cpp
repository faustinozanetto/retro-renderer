#include "rtpch.h"
#include "physics_material_component.h"

namespace retro::scene
{
    physics_material_component::physics_material_component(const std::shared_ptr<physics::physics_material> &physics_material)
    {
        m_physics_material = physics_material;
    }

    physics_material_component::~physics_material_component()
    {
    }
}