#pragma once

namespace retro::renderer
{
    class vertex_array_object
    {
    public:
        vertex_array_object();
        ~vertex_array_object();

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }

        void bind();
        void un_bind();

    private:
        uint32_t m_handle_id;
    };
}