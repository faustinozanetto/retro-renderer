#include "rtpch.h"
#include "scene_manager.h"

namespace retro::scene
{
    scene_manager::scene_manager()
    {
    }

    void scene_manager::set_active_scene(const std::shared_ptr<scene> &scene)
    {
        RT_PROFILE_SECTION("scene_manager::set_active_scene");
        m_active_scene = scene;
        RT_TRACE("Retro Renderer | Active scene changed to '{}'!", scene->get_name());
    }

	void scene_manager::create_scene(const std::string& name)
	{
        m_active_scene = std::make_shared<scene>(name);
	}
}