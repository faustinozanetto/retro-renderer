#pragma once

#include "utils/uuid.h"

namespace retro::scene
{
    class base_component
    {
    public:
		virtual ~base_component() = default;

        /* Getters */
        const uuid &get_uuid() const { return m_uuid; }

    protected:
        uuid m_uuid;
    };
}