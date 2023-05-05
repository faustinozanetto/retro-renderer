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
        shader();

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }

    private:
        uint32_t m_handle_id;
    };
}