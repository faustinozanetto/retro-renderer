#include "physx_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <core/entry_point.h>
#include <glm/gtc/type_ptr.hpp>

#include <utils/files.h>

#include <retro.h>
#include <imgui.h>

struct physx_comp
{
    physx::PxRigidDynamic *body;
};

physx_app::physx_app() : application("./")
{
    load_shaders();
    setup_camera();
    setup_frame_buffers();
    setup_screen_quad();

    retro::renderer::renderer::set_vsync_enabled(false);

    m_scene = std::make_shared<retro::scene::scene>("physx scene");
    retro::scene::scene_manager::get().set_active_scene(m_scene);

    m_physics_world = std::make_shared<retro::physics::physics_world>();

    auto *material = m_physics_world->get_physics()->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxRigidStatic *groundPlane = PxCreatePlane(*m_physics_world->get_physics(), physx::PxPlane(0, 1, 0, 50), *material);
    m_physics_world->get_scene()->addActor(*groundPlane);

    const auto &model = retro::renderer::model_loader::load_model_from_file("../resources/models/cube.obj");
    const auto &model_material = retro::renderer::material_loader::load_material_from_file("../resources/materials/test.rrm");

    float halfExtent = .2f;
    const std::shared_ptr<retro::physics::physics_box_collision>& collision_shape = std::make_shared<retro::physics::physics_box_collision>(m_physics_world->get_physics(),
        material, glm::vec3(halfExtent));
    physx::PxU32 size = 10;
    physx::PxTransform t(physx::PxVec3(0));
    for (physx::PxU32 i = 0; i < size; i++)
    {
        for (physx::PxU32 j = 0; j < size - i; j++)
        {
            physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1), 0) * halfExtent);
            physx::PxRigidDynamic *body = m_physics_world->get_physics()->createRigidDynamic(t.transform(localTm));
            body->attachShape(*collision_shape->get_physx_shape());
            physx::PxRigidBodyExt::updateMassAndInertia(*body, 15.0f);
            m_physics_world->get_scene()->addActor(*body);

            // Add actor in the render side.
            auto actor = m_scene->create_actor("physx actor");
            auto &transform_component = actor->add_component<retro::scene::transform_component>();
            transform_component.set_location({localTm.p.x, localTm.p.y, localTm.p.z});
            glm::quat rotation(localTm.q.w, localTm.q.x, localTm.q.y, localTm.q.z);
            transform_component.set_rotation(glm::eulerAngles(rotation));

            actor->add_component<retro::scene::model_renderer_component>(model);
            actor->add_component<retro::scene::material_renderer_component>(model_material);

            actor->add_component<physx_comp>().body = body;
        }
    }
}

physx_app::~physx_app()
{
}

void physx_app::on_update()
{
    m_physics_world->on_update();

	// 1. Render to geometry fbo
	retro::renderer::renderer::set_state(retro::renderer::renderer_state::blend, false);
	retro::renderer::renderer::set_state(retro::renderer::renderer_state::depth, true);
	m_geometry_fbo->bind();
	retro::renderer::renderer::clear_screen();
	m_geometry_shader->bind();

	const glm::mat4 viewMatrix = m_camera->get_view_matrix();
	const glm::mat4 projectionMatrix = m_camera->get_projection_matrix();

	m_geometry_shader->set_mat4("u_view", viewMatrix);
	m_geometry_shader->set_mat4("u_projection", projectionMatrix);

	const auto& view = m_scene->get_actors_registry()->view<retro::scene::transform_component, physx_comp, retro::scene::model_renderer_component, retro::scene::material_renderer_component>();
	for (auto&& [actor, transform_component, physx_component, model_renderer_component, material_renderer_component] : view.each())
	{
		// Update the cube's position and rotation
		const physx::PxTransform& actorTransform = physx_component.body->getGlobalPose();
		const physx::PxVec3& actorPosition = actorTransform.p;
		const physx::PxQuat& actorRotation = actorTransform.q;

		transform_component.set_location(glm::vec3(actorPosition.x, actorPosition.y, actorPosition.z));
		transform_component.set_rotation(glm::eulerAngles(glm::quat(actorRotation.w, actorRotation.x, actorRotation.y, actorRotation.z)));

		const glm::mat4& transformMatrix = transform_component.get_transform();
		m_geometry_shader->set_mat4("u_transform", transformMatrix);
        material_renderer_component.get_material()->set_albedo(physx_component.body->isSleeping() ? glm::vec3(1.0f, 0.0f, 0.f ) : glm::vec3(0.0f, 1.0f, 0.0f ));
        material_renderer_component.get_material()->bind(m_geometry_shader);
		retro::renderer::renderer::submit_model(model_renderer_component.get_model());
	}

	m_geometry_shader->un_bind();
	m_geometry_fbo->un_bind();
	retro::renderer::renderer::set_state(retro::renderer::renderer_state::depth, false);

	// 2. Render final result to screen
	retro::renderer::renderer::clear_screen();
	retro::renderer::renderer::set_viewport_size(retro::renderer::renderer::get_viewport_size());
	m_screen_shader->bind();
	retro::renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(1));
	m_screen_vao->bind();
	retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
	m_screen_vao->un_bind();
	m_screen_shader->un_bind();

	// 6. ImGui Debug
	retro::ui::engine_ui::begin_frame();

	ImGui::ShowMetricsWindow();

	glm::vec3 cam_pos = m_camera->get_position();
	if (ImGui::SliderFloat3("Camera Position", glm::value_ptr(cam_pos), -50.0f, 50.0f))
	{
		m_camera->set_position(cam_pos);
	}

	retro::ui::engine_ui::end_frame();
}

void physx_app::load_shaders()
{
    m_geometry_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/geometry.rrs");
    m_screen_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/screen.rrs");
}

void physx_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, -50.0f, 50.0f});
}

void physx_app::setup_screen_quad()
{
    const std::vector<float> quad_vertices = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    const std::vector<uint32_t> indices = {
        0, 3, 1, // first triangle
        1, 3, 2, // second triangle
    };

    size_t vertex_buffer_size = quad_vertices.size() * sizeof(&quad_vertices[0]);
    size_t index_buffer_size = indices.size() * sizeof(&indices[0]);

    m_screen_vao = std::make_shared<retro::renderer::vertex_array_object>();
    std::shared_ptr<retro::renderer::vertex_buffer_object> vertices_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);

    std::shared_ptr<retro::renderer::vertex_buffer_object> index_buffer = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements, indices.size());

    m_screen_vao->bind();
    vertices_vbo->bind();
    vertices_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, vertex_buffer_size,
                           quad_vertices.data());

    index_buffer->bind();
    index_buffer->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, index_buffer_size, indices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false},
        };

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor>
        vertices_vbo_layout_descriptor = std::make_shared<retro::renderer::vertex_buffer_layout_descriptor>(
            layout_elements);
    vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);

    m_screen_vao->add_vertex_buffer(vertices_vbo);
    m_screen_vao->set_index_buffer(index_buffer);
    m_screen_vao->un_bind();
}

void physx_app::setup_frame_buffers()
{
    // 1. Create geometry fbo.
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
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size}};
        retro::renderer::frame_buffer_attachment depth_attachment = {
            retro::renderer::texture_internal_format::depth_component32f,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge, viewport_size};
        m_geometry_fbo = std::make_shared<retro::renderer::frame_buffer>(
            attachments, viewport_size.x, viewport_size.y, depth_attachment);
        m_geometry_fbo->initialize();
    }
}

void physx_app::on_handle_event(retro::events::base_event &event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::key_pressed_event>(BIND_EVENT_FN(physx_app::on_key_pressed));
    dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(physx_app::on_window_resize));
}

bool physx_app::on_window_resize(retro::events::window_resize_event &resize_event)
{
    m_geometry_fbo->resize(resize_event.get_size(), true);
    return application::on_window_resize(resize_event);
}

bool physx_app::on_key_pressed(retro::events::key_pressed_event &key_pressed_event)
{
    return false;
}

retro::core::application *retro::core::create_application()
{
    return new physx_app();
}
