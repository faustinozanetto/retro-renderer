#pragma once

namespace retro::renderer
{
    enum class texture_type
    {
        normal,
        hdr
    };

    enum class texture_internal_format
    {
        rg,
        rgb,
        rgba,
        bgr,
        bgra,
        red,
        green,
        blue,
        alpha,
    };

    enum class texture_format
    {
        r8,
        r16,
        r16f,
        r32f,
        rg8,
        rg16,
        rg16f,
        rg32f,
        rgb8,
        rgb16,
        rgb16f,
        rgb32f,
        rgba8,
        rgba16,
        rgba16f,
        rgba32f,
        depth_component16,
        depth_component24,
        depth_component32,
        depth_component32f,
        stencil_index8
    };

    struct texture_formats
    {
        texture_format format;
        texture_internal_format internal_format;
    };

    struct raw_texture_data
    {
        int width;
        int height;
        int channels;
        texture_formats formats;
        texture_type type;
        void *data;

        raw_texture_data(int width, int height, int channels, texture_type type, void *data);
        raw_texture_data(int width, int height, int channels, texture_formats formats, texture_type type, void *data) : width(width), height(height), channels(channels), data(data), type(type), formats(formats) {}
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

        static std::string get_texture_format_to_string(texture_format format);
        static std::string get_texture_internal_format_to_string(texture_internal_format internal_format);

        static uint32_t get_texture_format_to_opengl(texture_format format);
        static uint32_t get_texture_internal_format_to_opengl(texture_internal_format internal_format);

        static texture_formats get_texture_formats_from_channel_count(int channel_count);

    private:
        int m_width;
        int m_height;
        int m_channels;
        int m_mipmap_levels;

        uint32_t m_handle_id;
        texture_type m_type;
        texture_formats m_formats;
    };
}