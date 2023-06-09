#pragma once

#include "renderer/text/text.h"
#include "renderer/text/font.h"

#include "scene/actors/components/base_component.h"

namespace retro::scene
{
    class text_renderer_component : public base_component
    {
    public:
        text_renderer_component(const std::shared_ptr<renderer::font> &font, const std::shared_ptr<renderer::text> &text);
        ~text_renderer_component() override;

        /* Getters */
        const std::shared_ptr<renderer::font> &get_font() const { return m_font; }
        const std::shared_ptr<renderer::text> &get_text() const { return m_text; }

        /* Setters */
        void set_font(const std::shared_ptr<renderer::font> &font) { m_font = font; }
        void set_text(const std::shared_ptr<renderer::text> &text) { m_text = text; }

    private:
        std::shared_ptr<renderer::font> m_font;
        std::shared_ptr<renderer::text> m_text;
    };
}