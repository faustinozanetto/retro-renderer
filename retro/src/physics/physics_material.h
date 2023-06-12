#pragma once

#include <PxPhysicsAPI.h>

namespace retro::physics
{
    class physics_material
    {
    public:
        physics_material(float static_friction = 0.5f, float dynamic_friction = 0.5f, float restitution = 0.6f);
        ~physics_material();

        /* Getters */
        physx::PxMaterial *get_physx_material() const { return m_material; }

    private:
        physx::PxMaterial *m_material;
    };
}