#pragma once

#include "scene/scene.h"

#include <entt/entt.hpp>

namespace retro::scene
{
    class scene_actor
    {
    public:
        scene_actor(entt::entity handle);

        /* Getters */
        entt::entity get_handle() const { return m_handle; }

        /* Setters */
        void set_scene(scene *scene) { m_scene = scene; }

        /* Functions */
        template <typename T, typename... Args>
        T &add_component(Args &&...args)
        {
            assert(!has_component<T>(), "Scene actor already has that component!");
            return m_scene->get_actors_registry()->emplace<T>(m_handle, std::forward<Args>(args)...);
        }

        template <typename T>
        T &get_component()
        {
            assert(has_component<T>(), "Scene actor does not have that component!");
            return m_scene->get_actors_registry()->get<T>(m_handle);
        }

        template <typename T>
        bool has_component()
        {
            return m_scene->get_actors_registry()->any_of<T>(m_handle);
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