#include "rtpch.h"
#include "physics_utils.h"

#include "actors/physics_dynamic_actor.h"
#include "collision_shapes/physics_box_collision.h"
#include "scene/scene_manager.h"
#include "scene/actors/scene_actor.h"
#include "scene/actors/components/transform_component.h"
#include "scene/actors/components/rendering/model_renderer_component.h"
#include "scene/actors/components/rendering/material_renderer_component.h"
#include "scene/actors/components/physics/physics_dynamic_actor_component.h"
#include "scene/actors/components/physics/physics_material_component.h"
#include "scene/actors/components/physics/collision_shapes/physics_box_collision_shape_component.h"
#include "scene/actors/components/physics/joints/physics_d6_joint_component.h"
#include "scene/actors/components/physics/joints/physics_revolute_joint_component.h"
#include "scene/actors/components/physics/joints/physics_prismatic_joint_component.h"
#include "scene/actors/components/physics/joints/physics_spherical_joint_component.h"
#include "renderer/models/model_loader.h"
#include "renderer/materials/material.h"
#include "renderer/materials/material_loader.h"
#include "physics_material.h"
#include "constraints/physics_d6_joint.h"
#include "constraints/physics_prismatic_joint.h"

#include <glm/ext/quaternion_float.hpp>
#include <random>
#include "constraints/physics_spherical_joint.h"

namespace retro::physics
{
    glm::vec2 physics_utils::convert_physx_vec2_to_glm(const physx::PxVec2 &physx_vec2)
    {
        return glm::vec2(physx_vec2.x, physx_vec2.y);
    }

    glm::vec3 physics_utils::convert_physx_vec3_to_glm(const physx::PxVec3 &physx_vec3)
    {
        return glm::vec3(physx_vec3.x, physx_vec3.y, physx_vec3.z);
    }

    glm::quat physics_utils::convert_physx_quat_to_glm(const physx::PxQuat &physx_quat)
    {
        return glm::quat(physx_quat.w, physx_quat.x, physx_quat.y, physx_quat.z);
    }

    physx::PxVec2 physics_utils::convert_glm_vec2_to_physx(const glm::vec2 &glm_vec2)
    {
        return physx::PxVec2(glm_vec2.x, glm_vec2.y);
    }

    physx::PxVec3 physics_utils::convert_glm_vec3_to_physx(const glm::vec3 &glm_vec3)
    {
        return physx::PxVec3(glm_vec3.x, glm_vec3.y, glm_vec3.z);
    }

    physx::PxQuat physics_utils::convert_glm_quat_to_physx(const glm::quat &glm_quat)
    {
        return physx::PxQuat(glm_quat.x, glm_quat.y, glm_quat.z, glm_quat.w);
    }

    physx::PxQuat physics_utils::convert_glm_vec3_to_physx_quat(const glm::vec3 &glm_vec3)
    {
        glm::quat glm_rotation = glm::quat(glm::radians(glm_vec3));

        return convert_glm_quat_to_physx(glm_rotation);
    }

    physx::PxTransform physics_utils::create_transform_from_glm(const glm::vec3 &position, const glm::quat &rotation)
    {
        return physx::PxTransform(convert_glm_vec3_to_physx(position), convert_glm_quat_to_physx(rotation));
    }

    const char* physics_utils::get_physx_d6_axis_to_string(physx::PxD6Axis::Enum axis)
    {
		switch (axis) {
		case physx::PxD6Axis::eX:
			return "X Axis";
		case physx::PxD6Axis::eY:
			return "Y Axis";
		case physx::PxD6Axis::eZ:
			return "Z Axis";
		case physx::PxD6Axis::eTWIST:
			return "Twist Axis";
		case physx::PxD6Axis::eSWING1:
			return "Swing 1 Axis";
		case physx::PxD6Axis::eSWING2:
			return "Swing 2 Axis";
		default:
			return "Unknown Axis";
		}
    }

    void physics_utils::create_chain(const glm::vec3 &start_location, const glm::vec3& link_scale, float separation, int length)
    {
		std::random_device rd;
		std::uniform_real_distribution<> rand_float(0.0f, 1.0f);

        std::shared_ptr<physics_dynamic_actor> previous_actor = nullptr;
        physx::PxVec3 offset(separation / 2, 0, 0);
        physx::PxTransform start_position(convert_glm_vec3_to_physx(start_location)); 
        physx::PxTransform local_transform = physx::PxTransform(offset);

        auto current_scene = scene::scene_manager::get().get_active_scene();

        const std::shared_ptr<physics_material>& phys_material = std::make_shared<physics_material>(0.5f, 0.5f, 0.6f);
        auto model = renderer::model_loader::load_model_from_file("../resources/models/cube.obj");

        for (int i = 0; i < length; ++i)
        {
			/* Box collision shape */
			const std::shared_ptr<physics_box_collision>& box_collision_shape = std::make_shared<physics_box_collision>(phys_material, link_scale);

			/* Setup physics dynamic actor */
			physx::PxTransform curr_trans = start_position * local_transform;
			const std::shared_ptr<physics_dynamic_actor>& dynamic_actor = std::make_shared<physics_dynamic_actor>(convert_physx_vec3_to_glm(curr_trans.p),
				convert_physx_quat_to_glm(curr_trans.q));
            dynamic_actor->add_collision_shape(box_collision_shape);
            dynamic_actor->initialize();

			/* Create the scene actor */
			const std::shared_ptr<scene::scene_actor>& scene_actor = current_scene->create_actor("chain actor");

			/* Setup transform */
			glm::vec3 location = convert_physx_vec3_to_glm(dynamic_actor->get_physx_rigid_dynamic_actor()->getGlobalPose().p);
			glm::quat rotation = convert_physx_quat_to_glm(dynamic_actor->get_physx_rigid_dynamic_actor()->getGlobalPose().q);
			scene_actor->add_component<scene::transform_component>(location, rotation, link_scale);

			/* Setup model renderer */
			scene_actor->add_component<scene::model_renderer_component>(model);

			/* Setup material renderer */
            glm::vec3 color = glm::vec3(rand_float(rd), rand_float(rd), rand_float(rd));
            renderer::material_data mat_data;
            mat_data.albedo = color;
            auto model_material = renderer::material_loader::load_material_from_data(mat_data);
			scene_actor->add_component<scene::material_renderer_component>(model_material);

			/* Setup physics components */
			scene_actor->add_component<scene::physics_dynamic_actor_component>(dynamic_actor);
			scene_actor->add_component<scene::physics_material_component>(phys_material);
			scene_actor->add_component<scene::physics_box_collision_shape_component>(box_collision_shape);

            /* Joint creation */
			const std::shared_ptr<physics_d6_joint>& joint = std::make_shared<physics_d6_joint>(previous_actor,
                previous_actor ? physx::PxTransform(offset) : start_position,
                dynamic_actor, physx::PxTransform(-offset));

			joint->set_motion(physx::PxD6Axis::eSWING1, physx::PxD6Motion::eFREE);
			joint->set_motion(physx::PxD6Axis::eSWING2, physx::PxD6Motion::eFREE);
			joint->set_motion(physx::PxD6Axis::eTWIST, physx::PxD6Motion::eFREE);
			joint->set_drive(physx::PxD6Drive::eSLERP, physx::PxD6JointDrive(10.0f, 300.0f, FLT_MAX, true));

			/* Setup joint component */
			scene_actor->add_component<scene::physics_d6_joint_component>(joint);

			// Update the position for the next actor
            previous_actor = dynamic_actor;
			local_transform.p.x += separation;
        }
    }
}