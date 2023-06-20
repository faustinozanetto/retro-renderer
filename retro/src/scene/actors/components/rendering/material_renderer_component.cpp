#include "rtpch.h"
#include "material_renderer_component.h"

#include "renderer/materials/material_loader.h"

namespace retro::scene
{
    material_renderer_component::material_renderer_component()
    {
        m_material = renderer::material_loader::load_material_default();
    }

    material_renderer_component::material_renderer_component(const std::shared_ptr<renderer::material> &material)
    {
        m_material = material;
    }

    material_renderer_component::~material_renderer_component()
    {
    }

    void material_renderer_component::initialize()
    {
    }
}
