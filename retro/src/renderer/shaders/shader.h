#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

namespace retro::renderer
{
    enum class shader_type
    {
        fragment,
        vertex,
        geometry,
    };

    class shader
    {
    public:
        shader(const std::unordered_map<shader_type, std::string> &shader_contents);

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }
        uint32_t get_uniform_location(const std::string &uniform_name);

        void bind();
        void un_bind();

        void set_int(const std::string &uniform_name, int value);
        void set_vec_int2(const std::string &uniform_name, const glm::ivec2 &value);
        void set_vec_int3(const std::string &uniform_name, const glm::ivec3 &value);
        void set_vec_int4(const std::string &uniform_name, const glm::ivec4 &value);
        void set_float(const std::string &uniform_name, float value);
        void set_vec_float2(const std::string &uniform_name, const glm::vec2 &value);
        void set_vec_float3(const std::string &uniform_name, const glm::vec3 &value);
        void set_vec_float4(const std::string &uniform_name, const glm::vec4 &value);
        void set_mat3(const std::string &uniform_name, const glm::mat3 &value);
        void set_mat4(const std::string &uniform_name, const glm::mat4 &value);

        /* Utilities */
        static uint32_t get_shader_type_to_opengl(shader_type type);
        static std::string get_shader_type_to_string(shader_type type);

    private:
        void compile_contents(const std::unordered_map<shader_type, std::string> &shader_contents);

        uint32_t m_handle_id;
        std::unordered_map<std::string, uint32_t> m_uniforms;
    };
}