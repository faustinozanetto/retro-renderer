#include "rtpch.h"
#include "scene.h"

#include "scene/actors/scene_actor.h"
#include "scene/actors/components/name_component.h"

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
        const auto &actor = std::make_shared<scene_actor>(actor_handle);
        actor->set_scene(this);
        return actor;
    }

    std::shared_ptr<scene_actor> scene::create_actor(const std::string &name)
    {
        entt::entity actor_handle = m_actors_registry->create();
        const auto &actor = std::make_shared<scene_actor>(actor_handle);
        actor->add_component<name_component>(name);
        actor->set_scene(this);
        return actor;
    }
}