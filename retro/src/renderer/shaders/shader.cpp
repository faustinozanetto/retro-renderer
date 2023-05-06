#include "rtpch.h"
#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace retro::renderer
{
    uint32_t shader::get_shader_type_to_opengl(shader_type type)
    {
        switch (type)
        {
        case shader_type::fragment:
            return GL_FRAGMENT_SHADER;
        case shader_type::vertex:
            return GL_VERTEX_SHADER;
        }
        RT_ASSERT_MSG(false, "Invalid shader type!");
    }

    std::string shader::get_shader_type_to_string(shader_type type)
    {
        switch (type)
        {
        case shader_type::fragment:
            return "fragment";
        case shader_type::vertex:
            return "vertex";
        }
        RT_ASSERT_MSG(false, "Invalid shader type!");
    }

    shader::shader(const std::unordered_map<shader_type, std::string> &shader_contents)
    {
        RT_TRACE("Retro Renderer | Started creating shader.");
        compile_contents(shader_contents);
        RT_TRACE("Retro Renderer | Shader created successfully.");
    }

    uint32_t shader::get_uniform_location(const std::string &uniform_name)
    {
        if (m_uniforms.contains(uniform_name))
            return m_uniforms[uniform_name];

        uint32_t location = glGetUniformLocation(m_handle_id, uniform_name.c_str());
        m_uniforms.insert(std::make_pair(uniform_name, location));
        return location;
    }

    void shader::bind()
    {
        glUseProgram(m_handle_id);
    }

    void shader::un_bind()
    {
        glUseProgram(0);
    }

    void shader::set_int(const std::string &uniform_name, int value)
    {
        glUniform1i(get_uniform_location(uniform_name), value);
    }

    void shader::set_vec_int2(const std::string &uniform_name, const glm::ivec2 &value)
    {
        glUniform2i(get_uniform_location(uniform_name), value.x, value.y);
    }

    void shader::set_vec_int3(const std::string &uniform_name, const glm::ivec3 &value)
    {
        glUniform3i(get_uniform_location(uniform_name), value.x, value.y, value.z);
    }

    void shader::set_vec_int4(const std::string &uniform_name, const glm::ivec4 &value)
    {
        glUniform4i(get_uniform_location(uniform_name), value.x, value.y, value.z, value.w);
    }

    void shader::set_float(const std::string &uniform_name, float value)
    {
        glUniform1f(get_uniform_location(uniform_name), value);
    }

    void shader::set_vec_float2(const std::string &uniform_name, const glm::vec2 &value)
    {
        glUniform2f(get_uniform_location(uniform_name), value.x, value.y);
    }

    void shader::set_vec_float3(const std::string &uniform_name, const glm::vec3 &value)
    {
        glUniform3f(get_uniform_location(uniform_name), value.x, value.y, value.z);
    }

    void shader::set_vec_float4(const std::string &uniform_name, const glm::vec4 &value)
    {
        glUniform4f(get_uniform_location(uniform_name), value.x, value.y, value.z, value.w);
    }

    void shader::set_mat3(const std::string &uniform_name, const glm::mat3 &value)
    {
        glUniformMatrix3fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void shader::set_mat4(const std::string &uniform_name, const glm::mat4 &value)
    {
        glUniformMatrix4fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void shader::compile_contents(const std::unordered_map<shader_type, std::string> &shader_contents)
    {
        uint32_t shader_program = glCreateProgram();
        uint32_t shader_ids[2] = {};
        int shader_index = 0;

        for (auto &shader : shader_contents)
        {
            uint32_t shader_id = glCreateShader(get_shader_type_to_opengl(shader.first));
            const char *source_str = shader.second.c_str();
            glShaderSource(shader_id, 1, &source_str, nullptr);
            glCompileShader(shader_id);

            int success = 0;
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE)
            {
                int max_lenght = 0;
                glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_lenght);

                std::vector<char> infoLog(max_lenght);
                glGetShaderInfoLog(shader_id, max_lenght, &max_lenght, infoLog.data());

                glDeleteShader(shader_id);

                // Print error log.
                std::string error_message = std::string(
                    infoLog.begin(), infoLog.end());
                RT_ERROR("  - An error ocurred while compiling shader '{0}'", error_message);
                RT_ASSERT(false);
            }

            RT_TRACE("  - Shader '{0}' compiled successfully.", get_shader_type_to_string(shader.first));
            // Attach
            glAttachShader(shader_program, shader_id);
            shader_ids[shader_index] = shader_id;
            shader_index++;
        }

        m_handle_id = shader_program;
        glLinkProgram(m_handle_id);

        int success = 0;
        glGetProgramiv(m_handle_id, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            int max_lenght = 0;
            glGetProgramiv(m_handle_id, GL_INFO_LOG_LENGTH, &max_lenght);

            std::vector<char> infoLog(max_lenght);
            glGetProgramInfoLog(m_handle_id, max_lenght, &max_lenght, infoLog.data());

            glDeleteProgram(m_handle_id);

            for (uint32_t id : shader_ids)
            {
                glDeleteShader(id);
            }

            // Print error log.
            std::string error_message = std::string(
                infoLog.begin(), infoLog.end());
            RT_ERROR("  - An error ocurred while linking shader '{0}'", error_message);
            RT_ASSERT(false);
        }

        RT_TRACE("  - Shader compiled successfully.");

        // Cleanup.
        for (auto id : shader_ids)
        {
            glDetachShader(m_handle_id, id);
            glDeleteShader(id);
        }
    }
}