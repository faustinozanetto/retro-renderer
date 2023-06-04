#include "rtpch.h"
#include "sound_loader.h"

#include "utils/utils.h"

#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"

#include <stb_vorbis.c>

namespace retro::audio
{
    std::shared_ptr<sound> sound_loader::load_sound_from_file(const std::string& file_path)
    {
        sound_data data;
        const std::string& file_extension = utils::extract_file_extansion(file_path);

        if (file_extension == ".ogg")
        {
            data = parse_ogg_sound(file_path);
        }
        else if (file_extension == ".wav")
        {
            data = parse_wav_sound(file_path);
        }

        return std::make_shared<sound>(file_path, data);
    }

    sound_data sound_loader::parse_ogg_sound(const std::string& file_path)
    {
        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        RT_ASSERT_MSG(file, "An error occurred while reading vorbis file!");

        std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(file_size);
        if (!file.read(buffer.data(), file_size))
        {
            // Error handling
            // ...
        }

        int error = 0;
        int num_channels = 0;
        int sample_rate = 0;
        short* samples = nullptr;
        int num_samples = stb_vorbis_decode_memory(reinterpret_cast<const unsigned char*>(buffer.data()),
                                                   static_cast<int>(buffer.size()), &num_channels, &sample_rate,
                                                   &samples);

        RT_ASSERT_MSG(error == VORBIS__no_error, "An error occurred while reading vorbis file!");
        RT_ASSERT_MSG(num_samples > 0, "An error occurred while reading vorbis file!");

        sound_data sound;
        sound.frequency = static_cast<float>(sample_rate);
        sound.length = static_cast<float>(num_samples) / static_cast<float>(sample_rate);
        sound.bit_rate = 16; // Assuming 16-bit samples
        sound.channels = num_channels;
        sound.size = num_samples * num_channels * sizeof(short);

        sound.data = new unsigned char[sound.size];
        std::memcpy(sound.data, samples, sound.size);

        return sound;
    }

    sound_data sound_loader::parse_wav_sound(const std::string& file_path)
    {
        sound_data data;

        // Use dr_wav library to load the WAV file
        drwav wav;
        bool init_result = drwav_init_file(&wav, file_path.c_str(), nullptr);
        RT_ASSERT_MSG(init_result, "Could not open sound file: {0}", m_file_path.c_str());

        data.frequency = static_cast<float>(wav.sampleRate);
        data.length = static_cast<float>(wav.totalPCMFrameCount) / wav.sampleRate;
        data.bit_rate = wav.bitsPerSample;
        data.channels = wav.channels;
        data.size = static_cast<int>(wav.totalPCMFrameCount * wav.channels * (wav.bitsPerSample / 8));
        data.data = new unsigned char[data.size];

        drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, data.data);

        // Clean up the WAV file
        drwav_uninit(&wav);

        return data;
    }
}
