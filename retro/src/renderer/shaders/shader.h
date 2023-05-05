#pragma once

namespace retro::renderer
{
    enum class shader_type
    {
        fragment,
        vertex,
    };

    class shader
    {
    public:
        shader(const std::unordered_map<shader_type, std::string> &shader_contents);

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }

        static uint32_t get_shader_type_to_opengl(shader_type type);
        static std::string get_shader_type_to_string(shader_type type);

    private:
        void compile_contents(const std::unordered_map<shader_type, std::string> &shader_contents);

        uint32_t m_handle_id;
    };
}