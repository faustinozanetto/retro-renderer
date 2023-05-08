#pragma once

namespace retro::renderer
{
    struct raw_texture_data
    {
        void* data;
        int width;
        int height;
        int channels;
    };

    enum class texture_filtering
    {
        none,
        nearest,
        linear,
        nearest_mipmap_nearest,
        linear_mipmap_nearest,
        nearest_mipmap_linear,
        linear_mipmap_linear,
    };

    enum class texture_filtering_type
    {
        filter_min,
        filter_mag,
    };

    enum class texture_wrapping
    {
        none,
        repeat,
        mirrored_repeat,
        clamp_to_edge,
        clamp_to_border,
    };

    enum class texture_wrapping_type
    {
        wrap_s,
        wrap_t,
        wrap_r,
    };

    class texture
    {
    public:
        texture(const raw_texture_data &raw_texture_data);

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }

        void set_filtering(texture_filtering_type filtering_type, texture_filtering filtering);
        void set_wrapping(texture_wrapping_type wrapping_type, texture_wrapping wrapping);

        static std::string get_texture_filtering_to_string(texture_filtering filtering);
        static std::string get_texture_filtering_type_to_string(texture_filtering_type filtering_type);
        static std::string get_texture_wrapping_to_string(texture_wrapping wrapping);
        static std::string get_texture_wrapping_type_to_string(texture_wrapping_type wrapping_type);

        static uint32_t get_texture_filtering_to_opengl(texture_filtering filtering);
        static uint32_t get_texture_filtering_type_to_opengl(texture_filtering_type filtering_type);
        static uint32_t get_texture_wrapping_to_opengl(texture_wrapping wrapping);
        static uint32_t get_texture_wrapping_type_to_opengl(texture_wrapping_type wrapping_type);

    private:
        void setup_texture_formats();

        int m_width;
        int m_height;
        int m_channels;
        int m_mipmap_levels;

        uint32_t m_handle_id;
        uint32_t m_format;
        uint32_t m_data_format;
    };
}