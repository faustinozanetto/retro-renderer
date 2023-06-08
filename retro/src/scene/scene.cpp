#include "rtpch.h"
#include "scene.h"

namespace retro::scene
{
    scene::scene() : scene("default_scene")
    {
    }

    scene::scene(const std::string &name)
    {
        RT_SEPARATOR();
        m_name = name;
        m_uuid = uuid();

        RT_TRACE("Retro Renderer | Started creating scene.");
        RT_TRACE("  - Name: '{}'", m_name);

        m_actors_registry = std::make_unique<entt::registry>();
        RT_TRACE("Retro Renderer | Scene created successfully!");
        RT_SEPARATOR();
    }

    scene::~scene()
    {
    }

    std::shared_ptr<scene_actor> scene::create_actor()
    {
        entt::entity actor_handle = m_actors_registry->create();
        return std::make_shared<scene_actor>(actor_handle);
    }
}