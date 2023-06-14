#pragma once

#include <string>
#include <glm/glm.hpp>

namespace retro::editor
{
    class editor_ui_utils
    {
    public:
        static void draw_property(const std::string &name);
        static void draw_property(const std::string &name, const std::string &content);
        static bool draw_property(const std::string &name, bool &value);
        static bool draw_property(const std::string &name, int &value, int min, int max);
        static bool draw_property(const std::string &name, float &value, float min, float max, float step = 0.1f);
        static bool draw_property(const std::string &name, glm::vec2 &value, float min, float max, float step = 0.1f);
        static bool draw_property(const std::string &name, glm::vec3 &value, float min, float max, float step = 0.1f);
        static bool draw_property(const std::string &name, glm::vec3 &value, bool color = true);
        static bool draw_property(const std::string &name, glm::vec4 &value, bool color = true);
    };
}
