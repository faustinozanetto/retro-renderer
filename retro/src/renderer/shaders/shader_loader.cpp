#include "rtpch.h"
#include "shader_loader.h"

#include "utils/utils.h"

#include <filesystem>

#include "shader_file_watcher.h"

namespace retro::renderer
{
    std::shared_ptr<shader> shader_loader::load_shader_from_file(const std::string &file_path)
    {
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading shader from file '{0}'", file_path);
        const std::string &shader_source = read_shader_from_file(file_path);
        const auto &contents = parse_shader_source(shader_source);

        auto created_shader = std::make_shared<shader>(file_path, contents);
        shader_file_watcher::get().create_shader_file_watcher(created_shader);
        RT_TRACE("Retro Renderer | Shader loaded from file successfully!");
        RT_SEPARATOR();
        return created_shader;
    }

    std::shared_ptr<shader> shader_loader::load_shader_from_contents(
        const std::unordered_map<shader_type, std::string>& contents)
    {
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading shader from contents");
        auto created_shader = std::make_shared<shader>("from_contents", contents);
        RT_TRACE("Retro Renderer | Shader loaded from contents successfully!");
        RT_SEPARATOR();
        return created_shader;
    }

    std::string shader_loader::read_shader_from_file(const std::string &file_path)
    {
        RT_TRACE("Retro Renderer | Started reading shader from file '{0}'", file_path);
        std::string shader_contents;
        std::ifstream in(file_path, std::ios::in | std::ios::binary);

        RT_ASSERT_MSG(in, "Retro Renderer | An error ocurred while opening shader file '{0}'", file_path);

        in.seekg(0, std::ios::end);
        const size_t size = in.tellg();
        RT_ASSERT_MSG(size != -1, "Retro Renderer | An error ocurred while reading shader file '{0}'", file_path);
        shader_contents.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(shader_contents.data(), size);

        in.close();
        RT_TRACE("Retro Renderer | Shader read successfully.");

        return shader_contents;
    }

    std::unordered_map<shader_type, std::string> shader_loader::parse_shader_source(const std::string &shader_source)
    {
        std::unordered_map<shader_type, std::string> shader_contents;

        const size_t shader_type_token_len = strlen(SHADER_TYPE_TOKEN);
        size_t pos = shader_source.find(SHADER_TYPE_TOKEN, 0);

        shader_type current_type = shader_type::fragment;

        while (pos != std::string::npos)
        {
            const size_t eol = shader_source.find_first_of("\r\n", pos);
            const size_t begin = pos + shader_type_token_len + 1;
            std::string type = shader_source.substr(begin, eol - begin);

            const size_t next_line_pos = shader_source.find_first_not_of("\r\n", eol);
            pos = shader_source.find(SHADER_TYPE_TOKEN, next_line_pos);

            if (type == shader::get_shader_type_to_string(shader_type::vertex))
                current_type = shader_type::vertex;
            else if (type == shader::get_shader_type_to_string(shader_type::fragment))
                current_type = shader_type::fragment;
            else if (type == shader::get_shader_type_to_string(shader_type::geometry))
                current_type = shader_type::geometry;

            shader_contents[current_type] = (pos == std::string::npos)
                                                ? shader_source.substr(next_line_pos)
                                                : shader_source.substr(next_line_pos, pos - next_line_pos);
        }

        return shader_contents;
    }
}
