#pragma once

#include <renderer/textures/texture.h>

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace retro::editor
{
    class editor_ui_utils
    {
    public:
        static void draw_property(const std::string &name);
        static void draw_property(const std::string &name, const std::string &content);
        static bool draw_property(const std::string &name, bool &value);
        static bool draw_property(const std::string &name, int &value, int min, int max, int reset_value = 1.0f);
        static bool draw_property(const std::string &name, float &value, float min, float max, float step = 0.1f, float reset_value = 1.0f);
        static bool draw_property(const std::string &name, glm::vec2 &value, float min, float max, float step = 0.1f, float reset_value = 1.0f);
        static bool draw_property(const std::string &name, glm::vec3 &value, float min, float max, float step = 0.1f, float reset_value = 1.0f);
        static bool draw_property(const std::string &name, glm::vec3 &value, bool color = true);
        static bool draw_property(const std::string &name, glm::vec4 &value, bool color = true);
        static bool draw_property(const std::string &name, const std::shared_ptr<renderer::texture>& texture);

        static void draw_combo_box(const std::string& name, int& selection_index, const std::vector<std::string>& items, std::function<void(int)> on_selected);
    };
}
