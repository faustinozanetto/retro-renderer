#pragma  once

#include "key_codes.h"

#include <glm/glm.hpp>

namespace retro
{
    class input
    {
    public:
        static bool is_key_pressed(key_code key_code);
        static glm::vec2 get_mouse_position();
    };
}
