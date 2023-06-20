#include "rtpch.h"
#include "physics_box_collision_shape_component.h"

#include "scene/actors/components/physics/physics_material_component.h"

namespace retro::scene
{
    physics_box_collision_shape_component::physics_box_collision_shape_component()
    {
    }

    void physics_box_collision_shape_component::initialize()
    {
        if (m_actor->has_component<physics_material_component>())
        {
            const auto& physics_material = m_actor->get_component<physics_material_component>();
            m_box_collision = std::make_shared<physics::physics_box_collision>(physics_material.get_physics_material());
        }
        else
        {
            const auto& physics_material = m_actor->add_component<physics_material_component>();
            m_box_collision = std::make_shared<physics::physics_box_collision>(physics_material.get_physics_material());
        }
    }
}
