#pragma once

namespace retro::renderer
{
    class uniform_buffer_object
    {
    public:
        uniform_buffer_object();
        ~uniform_buffer_object();

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }

        /* Setters */
        void set_data(uint32_t size, const void *data);

        /* Functions */
        void initialize(uint32_t size, const void* data);
        void bind(uint32_t slot);
        void un_bind();

    private:
        uint32_t m_handle_id;
        uint8_t *m_data;
        uint32_t m_size;
    };
}