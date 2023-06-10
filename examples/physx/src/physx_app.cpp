#include "physx_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <core/entry_point.h>
#include <glm/gtc/type_ptr.hpp>

#include <renderer/materials/material.h>
#include <utils/files.h>
#include <logging/logger.h>

physx_app::physx_app() : application("./")
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 12.0f});

    m_physics_world = std::make_shared<retro::physics::physics_world>();
}

physx_app::~physx_app()
{
}

void physx_app::on_update()
{
    m_physics_world->on_update();
}

void physx_app::on_handle_event(retro::events::base_event &event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::key_pressed_event>(BIND_EVENT_FN(physx_app::on_key_pressed));
    dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(physx_app::on_window_resize));
}

bool physx_app::on_window_resize(retro::events::window_resize_event &resize_event)
{

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
