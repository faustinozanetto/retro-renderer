#pragma once

#include "../../editor_actor_component_panel.h"

namespace retro::editor
{
    class editor_actor_physics_d6_joint_component_panel : public editor_actor_component_panel
    {
    public:
        editor_actor_physics_d6_joint_component_panel();

        std::pair<bool, size_t> get_actor_component_details() override;
        void on_render_component_details() override;

    private:
        void draw_d6_joint_motion_edit(const std::shared_ptr<physics::physics_d6_joint>& joint, physx::PxD6Axis::Enum axis, physx::PxD6Motion::Enum& type);
    };
}