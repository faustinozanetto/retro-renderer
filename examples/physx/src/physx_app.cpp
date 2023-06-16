#include "physx_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <core/entry_point.h>
#include <glm/gtc/type_ptr.hpp>
#include "physics/physics_utils.h"

#include <utils/files.h>

#include <retro.h>
#include <imgui.h>
#include <random>
#include "physics/physics_world.h"

physx_app::physx_app() : application("./")
{
    m_initialized = false;
    load_shaders();
    setup_camera();
    setup_cube_vao();
    setup_fbo();
    setup_screen_quad();
    m_move_camera = false;

    retro::renderer::renderer::set_vsync_enabled(false);

    m_scene = std::make_shared<retro::scene::scene>("physx scene");
    retro::scene::scene_manager::get().set_active_scene(m_scene);

    auto model = retro::renderer::model_loader::load_model_from_file("../resources/models/cube.obj");
    auto sphere_model = retro::renderer::model_loader::load_model_from_file("../resources/models/sphere.obj");
    auto model_material = retro::renderer::material_loader::load_material_from_file(
        "../resources/materials/test.rrm");
    auto model_material2 = retro::renderer::material_loader::load_material_from_file(
        "../resources/materials/test2.rrm");

    const std::shared_ptr<retro::physics::physics_material> &physics_material = std::make_shared<retro::physics::physics_material>(0.5f, 0.5f, 0.6f);

    const std::shared_ptr<retro::physics::physics_plane_collision> &plane_collision_shape = std::make_shared<retro::physics::physics_plane_collision>(physics_material);

    physx::PxRigidStatic *plane_static_actor = PxCreatePlane(*retro::physics::physics_world::get().get_physics(), physx::PxPlane(0, 1, 0, 50), *physics_material->get_physx_material());

    physx::PxTransform plane_transform = plane_static_actor->getGlobalPose();

    glm::vec3 plane_actor_location = retro::physics::physics_utils::convert_physx_vec3_to_glm(plane_transform.p);
    plane_actor_location.y -= 1.0f;
    glm::quat player_actor_rotation = retro::physics::physics_utils::convert_physx_quat_to_glm(plane_transform.q);

    const std::shared_ptr<retro::physics::physics_static_actor> &floor_physics_static_actor = std::make_shared<retro::physics::physics_static_actor>(plane_static_actor);
    floor_physics_static_actor->add_collision_shape(plane_collision_shape);
    floor_physics_static_actor->initialize();

    /* Setup plane actor */
    auto plane_actor = m_scene->create_actor("physx plane");
    plane_actor->add_component<retro::scene::transform_component>(plane_actor_location, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(300.0f, 1.0f, 300.0f));
    plane_actor->add_component<retro::scene::model_renderer_component>(model);
    plane_actor->add_component<retro::scene::material_renderer_component>(model_material2);
    plane_actor->add_component<retro::scene::physics_static_actor_component>(floor_physics_static_actor);

    // Create chain with fixed joints
    for (int x = 0; x < 10; x++)
    {
        for (int z = 0; z < 10; z++)
        {
            retro::physics::physics_utils::create_chain({ x * 10.0f, 15.0f, z * 10.0f }, {2.0f, 0.5f, 0.5f}, 4.0f);
        }
    }
    m_initialized = true;
}

physx_app::~physx_app()
{
}

void physx_app::on_update()
{
    if (!m_initialized) return; 

    retro::physics::physics_world::get().on_update();

    // 1. Render to geometry fbo
    m_geometry_fbo->bind();
    retro::renderer::renderer::set_state(retro::renderer::renderer_state::blend, false);
    retro::renderer::renderer::set_state(retro::renderer::renderer_state::depth, true);
    retro::renderer::renderer::clear_screen();
    m_geometry_shader->bind();

    const glm::mat4 &viewMatrix = m_camera->get_view_matrix();
    const glm::mat4 &projectionMatrix = m_camera->get_projection_matrix();

    m_geometry_shader->set_mat4("u_view", viewMatrix);
    m_geometry_shader->set_mat4("u_projection", projectionMatrix);

    const auto &view = m_scene->get_actors_registry()->view<retro::scene::transform_component, retro::scene::model_renderer_component, retro::scene::material_renderer_component>();
    for (auto &&[actor, transform_comp, model_renderer_comp, material_renderer_comp] :
         view.each())
    {
        // 1. Update dynamic physics actors
        if (m_scene->get_actors_registry()->any_of<retro::scene::physics_dynamic_actor_component>(actor))
        {
            auto &physics_dynamic_actor_comp = m_scene->get_actors_registry()->get<retro::scene::physics_dynamic_actor_component>(actor);
            if (physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->isSleeping())
                continue;

            glm::vec3 updated_location = retro::physics::physics_utils::convert_physx_vec3_to_glm(physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->getGlobalPose().p);
            glm::quat updated_rotation = retro::physics::physics_utils::convert_physx_quat_to_glm(physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->getGlobalPose().q);

            // Update the cube's position and rotation
            transform_comp.get_transform()->set_location(updated_location);
            transform_comp.get_transform()->set_rotation(updated_rotation);
        }

        // Render
        const glm::mat4 &transformMatrix = transform_comp.get_transform()->get_transform();
        m_geometry_shader->set_mat4("u_transform", transformMatrix);
        material_renderer_comp.get_material()->bind(m_geometry_shader);
        retro::renderer::renderer::submit_model(model_renderer_comp.get_model());
    }

    m_geometry_shader->un_bind();
    m_geometry_fbo->un_bind();

    // 5. Render final result to screen
    retro::renderer::renderer::clear_screen();
    retro::renderer::renderer::set_viewport_size(retro::renderer::renderer::get_viewport_size());
    m_screen_shader->bind();
    retro::renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(1));
    m_screen_vao->bind();
    retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
    m_screen_vao->un_bind();
    m_screen_shader->un_bind();

    retro::ui::engine_ui::begin_frame();
    ImGui::ShowMetricsWindow();
    retro::ui::engine_ui::end_frame();
}

void physx_app::load_shaders()
{
    m_physics_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/physics.rrs");
    m_geometry_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/geometry.rrs");
    m_screen_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/screen.rrs");
}

void physx_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({-70.0f, -15.0f, 60.0f});
    m_camera->set_yaw(-45.0f);
    m_camera->update_vectors();
    m_camera->construct_matrices();

    m_camera_controller = std::make_shared<retro::camera::camera_controller>(m_camera);
}

void physx_app::setup_fbo()
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

void physx_app::setup_cube_vao()
{
    std::vector<float> vertices = {
        // Front face
        -0.5f, -0.5f, 0.5f, // Bottom-left
        0.5f, -0.5f, 0.5f,  // Bottom-right
        0.5f, 0.5f, 0.5f,   // Top-right
        -0.5f, 0.5f, 0.5f,  // Top-left

        // Back face
        -0.5f, -0.5f, -0.5f, // Bottom-left
        0.5f, -0.5f, -0.5f,  // Bottom-right
        0.5f, 0.5f, -0.5f,   // Top-right
        -0.5f, 0.5f, -0.5f,  // Top-left

        // Left face
        -0.5f, -0.5f, -0.5f, // Bottom-front
        -0.5f, 0.5f, -0.5f,  // Top-front
        -0.5f, 0.5f, 0.5f,   // Top-back
        -0.5f, -0.5f, 0.5f,  // Bottom-back

        // Right face
        0.5f, -0.5f, 0.5f,  // Bottom-back
        0.5f, 0.5f, 0.5f,   // Top-back
        0.5f, 0.5f, -0.5f,  // Top-front
        0.5f, -0.5f, -0.5f, // Bottom-front

        // Top face
        -0.5f, 0.5f, 0.5f,  // Front-left
        0.5f, 0.5f, 0.5f,   // Front-right
        0.5f, 0.5f, -0.5f,  // Back-right
        -0.5f, 0.5f, -0.5f, // Back-left

        // Bottom face
        -0.5f, -0.5f, -0.5f, // Front-left
        0.5f, -0.5f, -0.5f,  // Front-right
        0.5f, -0.5f, 0.5f,   // Back-right
        -0.5f, -0.5f, 0.5f,  // Back-left
    };

    std::vector<uint32_t> indices = {
        // Front face
        0,
        1,
        2,
        2,
        3,
        0,

        // Back face
        4,
        5,
        6,
        6,
        7,
        4,

        // Left face
        8,
        9,
        10,
        10,
        11,
        8,

        // Right face
        12,
        13,
        14,
        14,
        15,
        12,

        // Top face
        16,
        17,
        18,
        18,
        19,
        16,

        // Bottom face
        20,
        21,
        22,
        22,
        23,
        20,
    };

    size_t vertex_buffer_size = vertices.size() * sizeof(&vertices[0]);
    size_t index_buffer_size = indices.size() * sizeof(&indices[0]);

    m_cube_vao = std::make_shared<retro::renderer::vertex_array_object>();
    std::shared_ptr<retro::renderer::vertex_buffer_object> vertices_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);

    std::shared_ptr<retro::renderer::vertex_buffer_object> index_buffer = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements, indices.size());

    m_cube_vao->bind();
    vertices_vbo->bind();
    vertices_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, vertex_buffer_size,
                           vertices.data());

    index_buffer->bind();
    index_buffer->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, index_buffer_size, indices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false}};

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor>
        vertices_vbo_layout_descriptor = std::make_shared<retro::renderer::vertex_buffer_layout_descriptor>(
            layout_elements);
    vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);

    m_cube_vao->add_vertex_buffer(vertices_vbo);
    m_cube_vao->set_index_buffer(index_buffer);
    m_cube_vao->un_bind();
}

void physx_app::on_handle_event(retro::events::base_event &event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::key_pressed_event>(BIND_EVENT_FN(physx_app::on_key_pressed_event));
    dispatcher.dispatch<retro::events::key_released_event>(BIND_EVENT_FN(physx_app::on_key_released_event));
    dispatcher.dispatch<retro::events::mouse_moved_event>(BIND_EVENT_FN(physx_app::on_mouse_moved_event));
    dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(physx_app::on_window_resize));
}

bool physx_app::on_window_resize(retro::events::window_resize_event &resize_event)
{
    m_geometry_fbo->resize(resize_event.get_size());
    return application::on_window_resize(resize_event);
}

bool physx_app::on_key_pressed_event(retro::events::key_pressed_event &key_pressed_event)
{
    if (key_pressed_event.get_key_code() == retro::input::key::W)
    {
        m_camera_controller->proccess_keyboard_input(
            retro::camera::camera_keyboard_direction::forward);
        return true;
    }
    else if (key_pressed_event.get_key_code() == retro::input::key::S)
    {
        m_camera_controller->proccess_keyboard_input(
            retro::camera::camera_keyboard_direction::backward);
        return true;
    }
    else if (key_pressed_event.get_key_code() == retro::input::key::A)
    {
        m_camera_controller->proccess_keyboard_input(
            retro::camera::camera_keyboard_direction::left);
        return true;
    }
    else if (key_pressed_event.get_key_code() == retro::input::key::D)
    {
        m_camera_controller->proccess_keyboard_input(
            retro::camera::camera_keyboard_direction::right);
        return true;
    }
    else if (key_pressed_event.get_key_code() == retro::input::key::Q)
    {
        m_camera_controller->proccess_keyboard_input(
            retro::camera::camera_keyboard_direction::down);
        return true;
    }
    else if (key_pressed_event.get_key_code() == retro::input::key::E)
    {
        m_camera_controller->proccess_keyboard_input(
            retro::camera::camera_keyboard_direction::up);
        return true;
    }
    else if (key_pressed_event.get_key_code() == retro::input::key::LeftAlt)
    {
        m_move_camera = true;
        return true;
    }
    return false;
}

bool physx_app::on_key_released_event(retro::events::key_released_event &key_released_event)
{
    m_move_camera = false;
    return false;
}

bool physx_app::on_mouse_moved_event(retro::events::mouse_moved_event &mouse_moved_event)
{
    if (m_move_camera)
    {
        m_camera_controller->process_mouse_input(mouse_moved_event.get_mouse_x(), mouse_moved_event.get_mouse_y());
        return true;
    }
    return false;
}

retro::core::application *retro::core::create_application()
{
    return new physx_app();
}
