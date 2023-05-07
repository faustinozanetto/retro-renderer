#include "rtpch.h"
#include "light.h"

namespace retro::renderer
{
    std::string light::get_light_type_to_string(light_type type)
    {
        switch (type)
        {
        case light_type::point:
            return "point";
        case light_type::directional:
            return "directional";
        case light_type::spot:
            return "spot";
        }
        RT_ASSERT_MSG(false, "Invalid light type!");
    }
}