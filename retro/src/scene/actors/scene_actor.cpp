#include "rtpch.h"
#include "scene_actor.h"

namespace retro::scene
{
    scene_actor::scene_actor()
    {
        m_handle = entt::null;
    }

    scene_actor::scene_actor(entt::entity handle)
    {
        m_handle = handle;
    }

    scene_actor::scene_actor(entt::entity handle, scene* scene)
    {
        m_handle = handle;
        m_scene = scene;
    }
}
