#include "editor_actor_details_panel.h"

#include "components/common/editor_actor_name_component_panel.h"
#include "components/common/editor_actor_transform_component_panel.h"
#include "components/renderer/editor_actor_model_renderer_component_panel.h"
#include "components/renderer/editor_actor_material_renderer_component_panel.h"
#include "components/physics/editor_actor_physics_material_component_panel.h"
#include "components/physics/actors/editor_actor_physics_static_actor_component_panel.h"
#include "components/physics/actors/editor_actor_physics_dynamic_actor_component_panel.h"
#include "components/physics/collision_shapes/editor_actor_physics_box_collision_component_panel.h"
#include "components/physics/collision_shapes/editor_actor_physics_plane_collision_component_panel.h"
#include "components/physics/collision_shapes/editor_actor_physics_sphere_collision_component_panel.h"
#include "components/physics/collision_shapes/editor_actor_physics_capsule_collision_component_panel.h"
#include "components/physics/joints/editor_actor_physics_fixed_joint_component_panel.h"
#include "components/physics/joints/editor_actor_physics_revolute_joint_component_panel.h"
#include "components/physics/joints/editor_actor_physics_prismatic_joint_component_panel.h"
#include "components/physics/joints/editor_actor_physics_spherical_joint_component_panel.h"
#include "components/physics/joints/editor_actor_physics_d6_joint_component_panel.h"

#include "../../utils/editor_ui_utils.h"

#include "../../editor_main_layer.h"

#include <core/application.h>
#include <scene/scene_manager.h>

namespace retro::editor
{
	editor_actor_details_panel::editor_actor_details_panel()
	{
		initialize();
	}

	editor_actor_details_panel::~editor_actor_details_panel() {}

	void editor_actor_details_panel::initialize()
	{
		/* Common Components*/
		m_component_panels.push_back(std::make_shared<editor_actor_name_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_transform_component_panel>());
		/*  Renderer Components */
		m_component_panels.push_back(std::make_shared<editor_actor_model_renderer_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_material_renderer_component_panel>());
		/* Physics Components */
		m_component_panels.push_back(std::make_shared<editor_actor_physics_material_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_static_actor_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_dynamic_actor_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_box_collision_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_plane_collision_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_sphere_collision_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_capsule_collision_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_fixed_joint_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_revolute_joint_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_prismatic_joint_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_spherical_joint_component_panel>());
		m_component_panels.push_back(std::make_shared<editor_actor_physics_d6_joint_component_panel>());
	}

	void editor_actor_details_panel::on_render_panel()
	{
		auto current_scene = scene::scene_manager::get().get_active_scene();

		ImGui::Begin("Actor Details");
		if (editor_main_layer::s_selected_actor != entt::null)
		{
			// Draw all the component panels
			for (const auto &component_panel : m_component_panels)
			{
				component_panel->on_render_panel();
			}
		}
		else
		{
			ImGui::Text("No active actor!");
		}
		ImGui::End();
	}
}