#pragma once

#include <PxPhysicsAPI.h>

namespace retro::physics
{
    class physics_material
    {
    public:
        physics_material(float staticFriction, float dynamicFriction, float restitution);
        ~physics_material();

        /* Getters */
        physx::PxMaterial *get_physx_material() const { return m_material; }

    private:
        physx::PxMaterial *m_material;
    };
}