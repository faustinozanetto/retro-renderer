#pragma once

namespace retro::renderer
{
    class texture
    {
    public:
        texture(const std::string &file_path);

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }

    private:
        void parse_texture_file();

        std::string m_file_path;
        int m_width;
        int m_height;
        int m_channels;
        int m_mipmap_levels;

        uint32_t m_handle_id;
        uint32_t m_format;
        uint32_t m_data_format;
    };
}