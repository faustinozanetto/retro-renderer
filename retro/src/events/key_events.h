#pragma once

#include "base_event.h"
#include "input/input.h"

namespace retro::events
{
    class key_event : public base_event
    {
    public:
        key_event(const key_code key_code) : m_key_code(key_code)
        {
        }

        key_code get_key_code() const { return m_key_code; }

    protected:
        key_code m_key_code;
    };

    class key_pressed_event : public key_event
    {
    public:
        key_pressed_event(const key_code key_code, bool repeat = false)
            : key_event(key_code), m_repeat(repeat)
        {
        }

        bool is_repeat() const { return m_repeat; }

        static event_type get_static_type() { return event_type::key_pressed; }
        virtual event_type get_type() const override { return event_type::key_pressed; }

    private:
        bool m_repeat;
    };

    class key_released_event : public key_event
    {
    public:
        key_released_event(const key_code key_code)
            : key_event(key_code)
        {
        }

        static event_type get_static_type() { return event_type::key_released; }
        virtual event_type get_type() const override { return event_type::key_released; }
    };
};
