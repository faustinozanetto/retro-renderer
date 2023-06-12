#pragma once

#include "base_event.h"
#include "input/input_manager.h"

namespace retro::events
{
    class mouse_moved_event : public base_event
    {
    public:
        mouse_moved_event(float x, float y) : m_pos_x(x), m_pos_y(y)
        {
        }

        /* Getters */
        float get_mouse_x() const { return m_pos_x; }
        float get_mouse_y() const { return m_pos_y; }

        /* Event */
        static event_type get_static_type() { return event_type::mouse_moved; }
        virtual event_type get_type() const override { return event_type::mouse_moved; }

    private:
        float m_pos_x;
        float m_pos_y;
    };

    class mouse_scrolled_event : public base_event
    {
    public:
        mouse_scrolled_event(float x_offset, float y_offset) : m_offset_x(x_offset), m_offset_y(y_offset)
        {
        }

        /* Getters */
        float get_offset_x() const { return m_offset_x; }
        float get_offset_y() const { return m_offset_y; }

        /* Event */
        static event_type get_static_type() { return event_type::mouse_scrolled; }
        virtual event_type get_type() const override { return event_type::mouse_scrolled; }

    private:
        float m_offset_x;
        float m_offset_y;
    };

    class mouse_button_event : public base_event
    {
    public:
        mouse_button_event(const input::mouse_button button_code)
            : m_button_code(button_code)
        {
        }

        /* Getters */
        input::mouse_button get_buton_code() const { return m_button_code; }

    protected:
        input::mouse_button m_button_code;
    };

    class mouse_button_pressed_event : public mouse_button_event
    {
    public:
        mouse_button_pressed_event(const input::mouse_button button_code) : mouse_button_event(button_code)
        {
        }

        /* Event */
        static event_type get_static_type() { return event_type::mouse_button_pressed; }
        virtual event_type get_type() const override { return event_type::mouse_button_pressed; }
    };

    class mouse_button_released_event : public mouse_button_event
    {
    public:
        mouse_button_released_event(const input::mouse_button button_code) : mouse_button_event(button_code)
        {
        }

        /* Event */
        static event_type get_static_type() { return event_type::mouse_button_released; }
        virtual event_type get_type() const override { return event_type::mouse_button_released; }
    };
};
