#pragma once

#include "scene/scene.h"

#include <entt/entt.hpp>

namespace retro::scene
{
    class scene_actor
    {
    public:
        scene_actor();
        scene_actor(entt::entity handle);
        scene_actor(entt::entity handle, scene* scene);
        scene_actor(const scene_actor &other) = default;

        /* Getters */
        entt::entity get_handle() const { return m_handle; }

        /* Setters */
        void set_scene(scene *scene) { m_scene = scene; }

        /* Functions */
        template <typename T, typename... Args>
        T &add_component(Args &&...args)
        {
            RT_ASSERT_MSG(!has_component<T>(), "Scene actor already has that component!");
            auto& component = m_scene->get_actors_registry()->emplace<T>(m_handle, std::forward<Args>(args)...);
            component.set_actor(this);
            component.initialize();
            return component;
        }

        template <typename T>
        T &get_component()
        {
            RT_ASSERT_MSG(has_component<T>(), "Scene actor does not have that component!");
            return m_scene->get_actors_registry()->get<T>(m_handle);
        }

        template <typename T>
        bool has_component() const
        {
            return m_scene->get_actors_registry()->any_of<T>(m_handle);
        }

        template <typename T>
        void remove_component() const
        {
            RT_ASSERT_MSG(has_component<T>(), "Scene actor does not have that component!");
            m_scene->get_actors_registry()->remove<T>(m_handle);
        }

        /* Operators */
        operator bool() const { return m_handle != entt::null; }

        bool operator==(const scene_actor &other) const
        {
            return m_handle == other.m_handle && m_scene == other.m_scene;
        }

        bool operator!=(const scene_actor &other) const
        {
            return !(*this == other);
        }

    private:
        entt::entity m_handle;
        scene *m_scene;
    };
}