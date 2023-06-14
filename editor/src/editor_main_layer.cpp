#include "editor_main_layer.h"

#include "ui/engine_ui.h"

#include "panels/editor_viewport_panel.h"
#include "panels/editor_toolbar_panel.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace retro::editor
{
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

        // Initialization
        setup_scene();
        setup_camera();

        m_model = renderer::model_loader::load_model_from_file("../resources/models/radio/radio.obj");
        m_material = renderer::material_loader::load_material_from_file("../resources/materials/radio.rrm");
        m_shader = renderer::shader_loader::load_shader_from_file("../resources/shaders/geometry.rrs");

		glm::ivec2 viewport_size = retro::renderer::renderer::get_viewport_size();
		{
			std::vector<retro::renderer::frame_buffer_attachment> attachments = {
				//  Position
				{
					retro::renderer::texture_internal_format::rgba16f,
					retro::renderer::texture_filtering::linear,
					retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
					// Albedo
					{
						retro::renderer::texture_internal_format::rgba16f,
						retro::renderer::texture_filtering::linear,
						retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
						// Normals
						{
							retro::renderer::texture_internal_format::rgba16f,
							retro::renderer::texture_filtering::linear,
							retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
							// Roughness Metallic AO
							{
								retro::renderer::texture_internal_format::rgba16f,
								retro::renderer::texture_filtering::linear,
								retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
								// Emissive
								{
									retro::renderer::texture_internal_format::rgba16f,
									retro::renderer::texture_filtering::linear,
									retro::renderer::texture_wrapping::clamp_to_edge, viewport_size} };
			retro::renderer::frame_buffer_attachment depth_attachment = {
				retro::renderer::texture_internal_format::depth_component32f,
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_to_edge, viewport_size };
			m_geometry_fbo = std::make_shared<retro::renderer::frame_buffer>(
				attachments, viewport_size.x, viewport_size.y, depth_attachment);
			m_geometry_fbo->initialize();
		}
    }

    void editor_main_layer::setup_camera()
    {
        m_camera = std::make_shared<camera::camera>(camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
        m_camera->set_position({ 0.0f, 0.5f, 12.0f });
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
        retro::renderer::renderer::clear_screen();
        m_shader->bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, static_cast<float>(glfwGetTime()), { 0, 1, 0 });
        model = glm::scale(model, glm::vec3(0.3f));
		m_shader->set_mat4("u_transform", model);
		m_shader->set_mat4("u_view", m_camera->get_view_matrix());
		m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
        m_material->bind(m_shader);
		renderer::renderer::submit_model(m_model);
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