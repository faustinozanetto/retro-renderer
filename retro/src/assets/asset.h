#pragma once

#include "utils/uuid.h"

namespace retro::assets
{
    enum class asset_type
    {
        shader = 0,
        texture,
        model
    };

    class asset
    {
    public:
        asset(asset_type type, const std::string &name) : m_type(type), m_name(name), m_uuid(uuid())
        {
        }

        /* Getters */
        const asset_type& get_type() const { return m_type; }
        const uuid& get_uuid() const { return m_uuid; }
        const std::string &get_name() const { return m_name; }

        /* Functions */
        virtual void serialize(std::ofstream &asset_pack_file) = 0;

    protected:
        asset_type m_type;
        uuid m_uuid;
        std::string m_name;
    };
}
