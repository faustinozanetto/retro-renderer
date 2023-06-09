#pragma once

namespace retro::events
{
    enum class event_type
    {
        window_resize,
        key_pressed,
        key_released,
        mouse_moved,
        mouse_scrolled,
        mouse_button_pressed,
        mouse_button_released
    };

    class base_event
    {
    public:
        virtual ~base_event() = default;

        /* Getters */
        virtual event_type get_type() const = 0;

        bool m_handled = false;
    };
}