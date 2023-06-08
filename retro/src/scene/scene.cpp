#include "rtpch.h"
#include "scene.h"

namespace retro::scene
{
    scene::scene(const std::string &name)
    {
        m_name = name;
        m_uuid = uuid();

        m_actors_registry = std::make_unique<entt::registry>();
    }

    std::shared_ptr<scene_actor> scene::create_actor()
    {
        entt::entity actor_handle = m_actors_registry->create();
        return std::make_shared<scene_actor>(actor_handle);
    }
}