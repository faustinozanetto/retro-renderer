#include "rtpch.h"
#include "material_renderer_component.h"

namespace retro::scene
{
    material_renderer_component::material_renderer_component(const std::shared_ptr<renderer::material> &material)
    {
        m_material = material;
    }

    material_renderer_component::~material_renderer_component()
    {
    }

}