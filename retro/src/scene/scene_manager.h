#pragma once

#include "utils/singleton.h"
#include "scene/scene.h"

namespace retro::scene
{
    class scene_manager : public singleton<scene_manager>
    {
    public:
        scene_manager();

        /* Getters */
        const std::shared_ptr<scene> &get_active_scene() const { return m_active_scene; }

        /* Functions */
        void set_active_scene(const std::shared_ptr<scene> &scene);

    private:
        std::shared_ptr<scene> m_active_scene;
    };
}