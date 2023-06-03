#pragma once

namespace retro::audio
{
    struct sound_data
    {
        unsigned char* data;
        float frequency;
        float length;
        uint32_t bit_rate;
        int size;
        uint32_t channels;
    };
    class sound
    {
    public:
        sound(const std::string& file_path);
        ~sound();

        uint32_t get_sound_buffer() const { return m_sound_buffer; };

    private:
        sound_data load_sound_from_file() const;
        std::string get_file_extension(const std::string& file_path) const;
        
        std::string m_file_path;

        sound_data m_sound_data;
        uint32_t m_sound_buffer;
    };
}