#pragma once

#include "sound.h"

namespace retro::audio
{
    class sound_loader
    {
    public:
        static std::shared_ptr<sound> load_sound_from_file(const std::string &file_path);
        static std::shared_ptr<sound> load_ogg_sound_from_memory(const char* data, size_t size);
        static std::shared_ptr<sound> load_wav_sound_from_memory(const char* data, size_t size);
        
    private:
        static sound_data parse_ogg_sound(const std::string &file_path);
        static sound_data parse_wav_sound(const std::string& file_path);
    };
}
