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
        void set_scene(scene * scene) { s_scene = scene; }

        /* Functions */
        template <typename T, typename... Args>
        T &add_component(Args &&...args)
        {
            RT_ASSERT_MSG(!has_component<T>(), "Scene actor already has that component!");
            return s_scene->get_actors_registry().emplace<T>(m_handle, std::forward<Args>(args)...);
        }

        template <typename T>
        bool has_component()
        {
            return s_scene->get_actors_registry().any_of<T>(m_handle);
        }

    private:
        entt::entity m_handle;
        static scene *s_scene;
    };
}