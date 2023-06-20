#include "rtpch.h"
#include "text_renderer_component.h"

namespace retro::scene
{
    text_renderer_component::text_renderer_component(const std::shared_ptr<renderer::font> &font, const std::shared_ptr<renderer::text> &text)
    {
        m_font = font;
        m_text = text;
    }

    text_renderer_component::~text_renderer_component()
    {
    }

    void text_renderer_component::initialize()
    {
    }
}
