#include "rtpch.h"
#include "shader_loader.h"

namespace retro::renderer
{
    std::string shader_loader::read_shader_from_file(const std::string &file_path)
    {
        RT_TRACE("Retro Renderer | Started reading shader from file '{0}'", file_path);
        std::string shader_contents;
        std::ifstream in(file_path, std::ios::in | std::ios::binary);

        if (in)
        {
            in.seekg(0, std::ios::end);
            const size_t size = in.tellg();
            if (size != -1)
            {
                shader_contents.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(shader_contents.data(), size);
            }
            else
            {
                RT_ERROR("Retro Renderer | An error ocurred while reading shader file '{0}'", file_path);
                RT_ASSERT(false);
            }
        }
        else
        {
            RT_ERROR("Retro Renderer | An error ocurred while opening shader file '{0}'", file_path);
            RT_ASSERT(false);
        }
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

            if (type == "vertex")
                current_type = shader_type::vertex;
            else
                current_type = shader_type::fragment;

            shader_contents[current_type] = (pos == std::string::npos)
                                                ? shader_source.substr(next_line_pos)
                                                : shader_source.substr(next_line_pos, pos - next_line_pos);
        }

        return shader_contents;
    }
}
