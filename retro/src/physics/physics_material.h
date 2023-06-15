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
        float get_static_friction() const;
        float get_dynamic_friction() const;
        float get_restitution() const;
        physx::PxMaterial* get_physx_material() const { return m_material; }

        /* Setters */
        void set_static_friction(float static_friction);
        void set_dynamic_friction(float dynamic_friction);
        void set_restitution(float restitution);

    private:
        physx::PxMaterial *m_material;
    };
}