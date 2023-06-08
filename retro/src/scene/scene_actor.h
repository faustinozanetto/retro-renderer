#pragma once

#include <entt/entt.hpp>

namespace retro::scene
{
    class scene_actor
    {
    public:
        scene_actor(entt::entity handle);

    private:
        entt::entity m_handle;
    };
}