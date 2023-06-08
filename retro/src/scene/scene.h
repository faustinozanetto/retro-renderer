#pragma once

#include "utils/uuid.h"
#include "scene/scene_actor.h"

#include <entt/entt.hpp>

namespace retro::scene
{
    class scene
    {
    public:
        scene();
        scene(const std::string &name);
        ~scene();

        /* Getters */
        const std::string &get_name() const { return m_name; }
        std::unique_ptr<entt::registry> &get_actors_registry() { return m_actors_registry; }

        /* Functions */
        std::shared_ptr<scene_actor> create_actor();

    private:
        uuid m_uuid;
        std::string m_name;

        std::unique_ptr<entt::registry> m_actors_registry;
    };
}