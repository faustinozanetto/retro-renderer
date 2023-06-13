#include "rtpch.h"
#include "model_renderer_component.h"

namespace retro::scene
{
    model_renderer_component::model_renderer_component(const std::shared_ptr<renderer::model> &model)
    {
        m_model = model;
    }

    model_renderer_component::~model_renderer_component()
    {
    }

}