#pragma once

#include "scene/actors/components/base_component.h"

namespace retro::scene
{
    class name_component : public base_component
    {
    public:
        name_component(const std::string &name);
        ~name_component() override;

        void initialize() override;

        /* Getters */
        const std::string &get_name() const { return m_name; }

        /* Setters */
        void set_name(const std::string &name) { m_name = name; }

    private:
        std::string m_name;
    };
}