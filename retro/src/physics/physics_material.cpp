#include "rtpch.h"
#include "physics_material.h"

namespace retro::physics
{
    physics_material::physics_material(float staticFriction, float dynamicFriction, float restitution)
    {
        m_material = PxGetPhysics().createMaterial(staticFriction, dynamicFriction, restitution);
    }

    physics_material::~physics_material()
    {
    }

    float physics_material::get_static_friction() const
    {
        RT_PROFILE;
        return m_material->getStaticFriction();
    }

    float physics_material::get_dynamic_friction() const
    {
        RT_PROFILE;
        return m_material->getDynamicFriction();
    }

    float physics_material::get_restitution() const
    {
        RT_PROFILE;
        return m_material->getRestitution();
    }

    void physics_material::set_static_friction(float static_friction)
    {
        RT_PROFILE;
        m_material->setStaticFriction(static_friction);
    }

    void physics_material::set_dynamic_friction(float dynamic_friction)
    {
        RT_PROFILE;
        m_material->setDynamicFriction(dynamic_friction);
    }

    void physics_material::set_restitution(float restitution)
    {
        RT_PROFILE;
        m_material->setRestitution(restitution);
    }
}