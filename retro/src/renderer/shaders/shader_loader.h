#pragma once

#include "shader.h"

#define SHADER_TYPE_TOKEN "#shader"

namespace retro::renderer
{
    class shader_loader
    {
    public:
        static std::shared_ptr<shader> load_shader_from_file(const std::string& file_path);
        static std::shared_ptr<shader> load_shader_from_contents(
            const std::unordered_map<shader_type, std::string>& contents);

        static std::string read_shader_from_file(const std::string& file_path);
        static std::unordered_map<shader_type, std::string> parse_shader_source(const std::string& shader_source);
    };
}
