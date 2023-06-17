#include "editor_main_layer.h"

#include "ui/engine_ui.h"

#include "panels/editor_viewport_panel.h"
#include "panels/editor_toolbar_panel.h"
#include "panels/editor_camera_panel.h"
#include "panels/scene/editor_scene_hierarchy_panel.h"
#include "panels/actor/editor_actor_details_panel.h"
#include "panels/console/editor_console_panel.h"
#include "panels/editor_profiler_panel.h"
#include "panels/editor_renderer_panel.h"
#include "audio/sound_loader.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <profiling/profiling.h>

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
        RT_PROFILE;
        m_initialized = false;
        // Create editor panels
        m_panels.push_back(std::make_shared<editor_viewport_panel>());
        m_panels.push_back(std::make_shared<editor_toolbar_panel>());
        m_panels.push_back(std::make_shared<editor_camera_panel>());
        m_panels.push_back(std::make_shared<editor_scene_hierarchy_panel>());
        m_panels.push_back(std::make_shared<editor_actor_details_panel>());
        m_panels.push_back(std::make_shared<editor_profiler_panel>());
        m_panels.push_back(std::make_shared<editor_renderer_panel>());
        m_console_panel = std::make_shared<editor_console_panel>();
        m_panels.push_back(m_console_panel);

        // Initialization
        setup_scene();
        setup_camera();

        m_shader = renderer::shader_loader::load_shader_from_file("resources/shaders/geometry.rrs");

        std::shared_ptr<renderer::material> material;

        material = renderer::material_loader::load_material_from_file("resources/materials/radio.rrm");
        m_model = renderer::model_loader::load_model_from_file("resources/models/radio/radio.obj");

        const std::shared_ptr<physics::physics_material> &physics_material = std::make_shared<physics::physics_material>(0.5f, 0.5f, 0.6f);

        const std::shared_ptr<physics::physics_box_collision> &box_collision_shape = std::make_shared<physics::physics_box_collision>(physics_material);

        auto m_demo_actor = scene::scene_manager::get().get_active_scene()->create_actor("test actor");
        m_demo_actor->add_component<scene::transform_component>();
        m_demo_actor->add_component<scene::model_renderer_component>(m_model);
        m_demo_actor->add_component<scene::material_renderer_component>(material);
        m_demo_actor->add_component<scene::sound_emitter_component>();
        auto sound = audio::sound_loader::load_sound_from_file("resources/sounds/player_shoot.ogg");
        m_demo_actor->add_component<scene::sound_source_component>(sound);

        // Create test chain
        physics::physics_utils::create_chain({0.0f, 20.0f, 0.0f}, {0.5f, 0.125f, 0.125f}, 1.1f);

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
        m_initialized = true;
    }

    void editor_main_layer::setup_camera()
    {
        RT_PROFILE;
        m_camera = std::make_shared<camera::camera>(camera::camera_type::perspective, 55.0f, 0.01f, 1000.0f);
        m_camera->set_position({0.0f, 15.0f, 15.0f});
    }

    void editor_main_layer::setup_scene()
    {
        RT_PROFILE;
        scene::scene_manager::get().create_scene("editor scene");
    }

    void editor_main_layer::on_render()
    {
        RT_PROFILE;
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
            auto &transform = transform_comp.get_transform();
            // 1. Update dynamic physics actors
            if (scene::scene_manager::get().get_active_scene()->get_actors_registry()->any_of<retro::scene::physics_dynamic_actor_component>(actor))
            {
                auto &physics_dynamic_actor_comp = scene::scene_manager::get().get_active_scene()->get_actors_registry()->get<retro::scene::physics_dynamic_actor_component>(actor);
                if (physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->isSleeping())
                    continue;

                glm::vec3 updated_location = retro::physics::physics_utils::convert_physx_vec3_to_glm(physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->getGlobalPose().p);
                glm::quat updated_rotation = retro::physics::physics_utils::convert_physx_quat_to_glm(physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->getGlobalPose().q);

                // Update the cube's position and rotation
                transform->set_location(updated_location);
                transform->set_rotation(updated_rotation);
            }

            // Render
            m_shader->set_mat4("u_transform", transform->get_transform());
            material_renderer_comp.get_material()->bind(m_shader);
            renderer::renderer::submit_model(model_renderer_comp.get_model());
        }


        m_shader->un_bind();

		renderer::debug_renderer::begin_render(m_camera);
		renderer::debug_renderer::submit_line({ 2.0f,2.0f,2.0f }, { 4.0f, 3.0f, -5.0f }, { 1.0f,0.85f,0.85f });
        renderer::debug_renderer::submit_bounding_box(m_model->get_bounding_box(), { 1.0f, 0.85f, 0.65f });
		renderer::debug_renderer::end_render();

        m_geometry_fbo->un_bind();
        renderer::renderer::set_state(renderer::renderer_state::depth, false);


    }

    void editor_main_layer::on_update()
    {
        RT_PROFILE;
        if (!m_initialized)
            return;

        physics::physics_world::get().on_update();

        on_render();

        ui::engine_ui::begin_frame();
        begin_dockspace();
        // Render panels
        for (const auto &panel : m_panels)
        {
            panel->on_render_panel();
        }
        ImGui::ShowDemoWindow();
        end_dockspace();
        ui::engine_ui::end_frame();
    }

    void editor_main_layer::begin_dockspace()
    {
        RT_PROFILE;
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
        RT_PROFILE;
        ImGui::End();
    }
}