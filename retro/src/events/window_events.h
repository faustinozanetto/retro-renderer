#pragma once

#include "events/base_event.h"

#include <glm/glm.hpp>

namespace retro::events
{
    class window_resize_event : public base_event
    {
    public:
        window_resize_event(const glm::ivec2 &window_size) : m_window_size(window_size) {}

        /* Getters */
        static event_type get_static_type() { return event_type::window_resize; }
        virtual event_type get_type() const override { return event_type::window_resize; }

    private:
        glm::ivec2 m_window_size;
    };
}