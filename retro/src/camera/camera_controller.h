#pragma once

#include "camera/camera.h"

#define DEFAULT_CAMERA_SPEED 0.05f
#define DEFAULT_CAMERA_MOUSE_SPEED 0.01f

namespace retro::camera
{
    enum class camera_keyboard_direction
    {
        forward,
        backward,
        left,
        right,
        up,
        down,
    };

    class camera_controller
    {
    public:
        camera_controller(const std::shared_ptr<camera> &camera);

        void on_update();

        void set_camera(const std::shared_ptr<camera> &camera);

        void proccess_keyboard_input(camera_keyboard_direction direction);
        void process_mouse_input(float dx, float dy);

    private:
        std::shared_ptr<camera> m_camera;

        float m_last_mouse_x;
        float m_last_mouse_y;
        bool m_is_first_mouse;

        bool m_is_mouse_enabled;
        float m_mouse_sensitivity;
        float m_movement_speed;
    };
};