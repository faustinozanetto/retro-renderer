#include "editor_main_layer.h"

#include "ui/engine_ui.h"

#include "panels/editor_viewport_panel.h"
#include "panels/editor_toolbar_panel.h"
#include "panels/editor_camera_panel.h"
#include "panels/scene/editor_scene_hierarchy_panel.h"
#include "panels/actor/editor_actor_details_panel.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace retro::editor
{
    entt::entity editor_main_layer::s_selected_actor = entt::null;

    editor_main_layer::editor_main_layer()
    {
        initialize();
    }

    editor_main_layer::~editor_main_layer()
    {
    }

    void editor_main_layer::initialize()
    {
        // Create editor panels
        m_panels.push_back(std::make_shared<editor_viewport_panel>());
        m_panels.push_back(std::make_shared<editor_toolbar_panel>());
        m_panels.push_back(std::make_shared<editor_camera_panel>());
        m_panels.push_back(std::make_shared<editor_scene_hierarchy_panel>());
        m_panels.push_back(std::make_shared<editor_actor_details_panel>());

        // Initialization
        setup_scene();
        setup_camera();

        ImGui::StyleColorsLight();

        auto model = renderer::model_loader::load_model_from_file("../resources/models/radio/radio.obj");
        auto material = renderer::material_loader::load_material_from_file("../resources/materials/radio.rrm");
        m_shader = renderer::shader_loader::load_shader_from_file("../resources/shaders/geometry.rrs");

		const std::shared_ptr<physics::physics_material>& physics_material = std::make_shared<physics::physics_material>(0.5f, 0.5f, 0.6f);

		const std::shared_ptr<physics::physics_box_collision>& box_collision_shape = std::make_shared<physics::physics_box_collision>(physics_material);

        m_demo_actor = scene::scene_manager::get().get_active_scene()->create_actor("test actor");
        m_demo_actor->add_component<scene::transform_component>();
        m_demo_actor->add_component<scene::model_renderer_component>(model);
        m_demo_actor->add_component<scene::material_renderer_component>(material);
        m_demo_actor->add_component<scene::physics_box_collision_shape_component>(box_collision_shape);

        {

            const std::shared_ptr<physics::physics_plane_collision> &plane_collision_shape = std::make_shared<physics::physics_plane_collision>(physics_material);

            
                physx::PxRigidStatic* plane_static_actor = PxCreatePlane(*physics::physics_world::get().get_physics(), physx::PxPlane(0, 1, 0, 50), *physics_material->get_physx_material());


            physx::PxTransform plane_transform = plane_static_actor->getGlobalPose();

            glm::vec3 plane_actor_location = physics::physics_utils::convert_physx_vec3_to_glm(plane_transform.p);
            plane_actor_location.y -= 1.0f;
            glm::quat player_actor_rotation = physics::physics_utils::convert_physx_quat_to_glm(plane_transform.q);

            const std::shared_ptr<physics::physics_static_actor> &floor_physics_static_actor = std::make_shared<physics::physics_static_actor>(plane_static_actor);
            floor_physics_static_actor->add_collision_shape(plane_collision_shape);
            floor_physics_static_actor->initialize();

            /* Setup plane actor */
            auto plane_actor = scene::scene_manager::get().get_active_scene()->create_actor("physx plane");
            plane_actor->add_component<scene::transform_component>(plane_actor_location, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(300.0f, 1.0f, 300.0f));
            plane_actor->add_component<scene::model_renderer_component>(model);
            plane_actor->add_component<scene::material_renderer_component>(material);
            plane_actor->add_component<scene::physics_static_actor_component>(floor_physics_static_actor);
            plane_actor->add_component<scene::physics_material_component>(physics_material);
            plane_actor->add_component<scene::physics_plane_collision_shape_component>(plane_collision_shape);
        }

        glm::ivec2 viewport_size = renderer::renderer::get_viewport_size();
        {
            std::vector<renderer::frame_buffer_attachment> attachments = {
                //  Position
                {
                    renderer::texture_internal_format::rgba16f,
                    renderer::texture_filtering::linear,
                    renderer::texture_wrapping::clamp_to_edge, viewport_size},
                // Albedo
                {
                    renderer::texture_internal_format::rgba16f,
                    renderer::texture_filtering::linear,
                    renderer::texture_wrapping::clamp_to_edge, viewport_size},
                // Normals
                {
                    renderer::texture_internal_format::rgba16f,
                    renderer::texture_filtering::linear,
                    renderer::texture_wrapping::clamp_to_edge, viewport_size},
                // Roughness Metallic AO
                {
                    renderer::texture_internal_format::rgba16f,
                    renderer::texture_filtering::linear,
                    renderer::texture_wrapping::clamp_to_edge, viewport_size},
                // Emissive
                {
                    renderer::texture_internal_format::rgba16f,
                    renderer::texture_filtering::linear,
                    renderer::texture_wrapping::clamp_to_edge, viewport_size}};
            renderer::frame_buffer_attachment depth_attachment = {
                renderer::texture_internal_format::depth_component32f,
                renderer::texture_filtering::linear,
                renderer::texture_wrapping::clamp_to_edge, viewport_size};
            m_geometry_fbo = std::make_shared<renderer::frame_buffer>(
                attachments, viewport_size.x, viewport_size.y, depth_attachment);
            m_geometry_fbo->initialize();
        }
    }

    void editor_main_layer::setup_camera()
    {
        m_camera = std::make_shared<camera::camera>(camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
        m_camera->set_position({0.0f, 0.5f, 12.0f});
    }

    void editor_main_layer::setup_scene()
    {
        scene::scene_manager::get().create_scene("editor scene");
    }

    void editor_main_layer::on_render()
    {
        renderer::renderer::set_state(renderer::renderer_state::blend, false);
        renderer::renderer::set_state(renderer::renderer_state::depth, true);
        m_geometry_fbo->bind();
        renderer::renderer::clear_screen();
        m_shader->bind();
        m_shader->set_mat4("u_view", m_camera->get_view_matrix());
        m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());

        const auto &view = scene::scene_manager::get().get_active_scene()->get_actors_registry()->view<scene::transform_component, scene::model_renderer_component, scene::material_renderer_component>();
        for (auto &&[actor, transform_comp, model_renderer_comp, material_renderer_comp] :
             view.each())
        {
            // Render
            const glm::mat4 &transformMatrix = transform_comp.get_transform();
            m_shader->set_mat4("u_transform", transformMatrix);
            material_renderer_comp.get_material()->bind(m_shader);
            renderer::renderer::submit_model(model_renderer_comp.get_model());
        }

        m_shader->un_bind();
        m_geometry_fbo->un_bind();
        renderer::renderer::set_state(renderer::renderer_state::depth, false);
    }

    void editor_main_layer::on_update()
    {
        on_render();

        ui::engine_ui::begin_frame();
        begin_dockspace();
        // Render panels
        for (const auto &panel : m_panels)
        {
            panel->on_render_panel();
        }
        end_dockspace();
        ui::engine_ui::end_frame();
    }

    void editor_main_layer::begin_dockspace()
    {
        static bool open = true;
        static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        const float toolbar_height = ImGui::GetFrameHeight();
        ImGui::SetNextWindowPos({viewport->Pos.x, viewport->Pos.y + toolbar_height});
        ImGui::SetNextWindowSize({viewport->Size.x, viewport->Size.y - toolbar_height});
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &open, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockMain = ImGui::GetID("MyDockspace");
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGui::DockSpace(dockMain, ImVec2(0.0f, 0.0f), opt_flags);
        }
    }

    void editor_main_layer::end_dockspace()
    {
        ImGui::End();
    }
}