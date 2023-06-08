#include "rtpch.h"
#include "scene_actor.h"

namespace retro::scene
{
    scene *scene_actor::s_scene = nullptr;

    scene_actor::scene_actor(entt::entity handle)
    {
        m_handle = handle;
    }
}