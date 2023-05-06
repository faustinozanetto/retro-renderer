#pragma once

#include "events/base_event.h"

#include <functional>

namespace retro::events
{
    class event_dispatcher
    {
    public:
        event_dispatcher(base_event &event) : m_event(event) {}

        template <typename T>
        bool dispatch(std::function<bool(T &)> func)
        {
            if (m_event.get_type() == T::get_static_type())
            {
                m_event.m_handled = func(static_cast<T &>(m_event));
                return true;
            }
            return false;
        }

    private:
        base_event &m_event;
    };
}