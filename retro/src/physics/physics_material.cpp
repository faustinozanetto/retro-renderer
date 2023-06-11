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
}