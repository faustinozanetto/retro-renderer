#pragma once

#include "renderer/materials/material.h"

#include "scene/actors/components/base_component.h"

namespace retro::scene
{
    struct material_renderer_component : public base_component
    {
    public:
        material_renderer_component(const std::shared_ptr<renderer::material> &material);
        ~material_renderer_component() override;

        /* Getters */
        const std::shared_ptr<renderer::material> &get_material() const { return m_material; }

        /* Setters */
        void set_material(const std::shared_ptr<renderer::material> &material) { m_material = material; }

    private:
        std::shared_ptr<renderer::material> m_material;
    };
}