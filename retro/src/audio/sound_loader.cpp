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
        RT_PROFILE_SECTION("sound_loader::load_sound_from_file");
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading sound from file.");
        sound_data data;
        const std::string& file_extension = utils::extract_file_extension(file_path);

        if (file_extension == ".ogg")
        {
            data = parse_ogg_sound(file_path);
        }
        else if (file_extension == ".wav")
        {
            data = parse_wav_sound(file_path);
        }

        auto created_sound = std::make_shared<sound>(file_path, data);
        RT_TRACE("Retro Renderer | Sound loaded from file successfully!");
        RT_SEPARATOR();
        return created_sound;
    }

    std::shared_ptr<sound> sound_loader::load_ogg_sound_from_memory(const char* data, size_t size)
    {
        RT_PROFILE_SECTION("sound_loader::load_ogg_sound_from_memory");
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading ogg sound from memory.");
        int error = 0;
        int channels;
        int sample_rate;
        short* samples;
        int sample_count = stb_vorbis_decode_memory(reinterpret_cast<const unsigned char*>(data), size, &channels,
                                                    &sample_rate, &samples);

        RT_ASSERT_MSG(error == VORBIS__no_error, "An error occurred while reading vorbis file!");
        RT_ASSERT_MSG(sample_count > 0, "An error occurred while reading vorbis file!");

        sound_data sound_data;
        sound_data.frequency = static_cast<float>(sample_rate);
        sound_data.length = static_cast<float>(sample_count) / sample_rate;
        sound_data.bit_rate = 16; // Assuming 16-bit audio
        sound_data.channels = static_cast<uint32_t>(channels);
        sound_data.size = sample_count * channels * sizeof(short);
        sound_data.data = samples;

        auto created_sound = std::make_shared<sound>("from_memory", sound_data);
        RT_TRACE("Retro Renderer | Sound loaded from memory successfully!");
        RT_SEPARATOR();
        return created_sound;
    }

    std::shared_ptr<sound> sound_loader::load_wav_sound_from_memory(const char* data, size_t size)
    {
        RT_PROFILE_SECTION("sound_loader::load_wav_sound_from_memory");
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading wav sound from memory.");
        // Use dr_wav library to load the WAV file
        drwav wav;
        bool init_result = drwav_init_memory(&wav, data, size, nullptr);
        RT_ASSERT_MSG(init_result, "Could not open sound file: {0}", m_file_path.c_str());

        // Create sound metadata
        sound_data sound_data;
        sound_data.frequency = static_cast<float>(wav.sampleRate);
        sound_data.length = static_cast<float>(wav.totalPCMFrameCount) / wav.sampleRate;
        sound_data.bit_rate = wav.bitsPerSample;
        sound_data.channels = wav.channels;
        sound_data.size = static_cast<int>(wav.totalPCMFrameCount * wav.channels * (wav.bitsPerSample / 8));
        sound_data.data = new unsigned char[sound_data.size];

        // Read into data buffer
        drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, sound_data.data);

        // Clean up the WAV file
        drwav_uninit(&wav);

        auto created_sound = std::make_shared<sound>("from_memory", sound_data);
        RT_TRACE("Retro Renderer | Sound loaded from memory successfully!");
        RT_SEPARATOR();
        return created_sound;
    }

    sound_data sound_loader::parse_ogg_sound(const std::string& file_path)
    {
        RT_PROFILE_SECTION("sound_loader::parse_ogg_sound");
        FILE* file_handle = fopen(file_path.c_str(), "rb");
        if (!file_handle)
        {
            RT_ASSERT_MSG(false, "Could not open sound file: {0}", m_file_path.c_str());
        }

        int error_code;
        stb_vorbis* stream_handle =
            stb_vorbis_open_filename(file_path.c_str(), &error_code, nullptr);
        if (!stream_handle)
        {
            RT_ASSERT_MSG(false, "Could not read sound file: {0}", m_file_path.c_str());
        }

        sound_data sound_data;
        const stb_vorbis_info sound_information = stb_vorbis_get_info(stream_handle);
        sound_data.channels = sound_information.channels;
        sound_data.bit_rate = 16;
        sound_data.frequency = sound_information.sample_rate;
        const uint32_t size = stb_vorbis_stream_length_in_samples(stream_handle) *
            sound_data.channels * sizeof(int16_t);
        sound_data.size = size;
        sound_data.data = new unsigned char[sound_data.size];

        stb_vorbis_get_samples_short_interleaved(
            stream_handle, sound_data.channels,
            reinterpret_cast<short*>(sound_data.data), sound_data.size);
        sound_data.length = stb_vorbis_stream_length_in_seconds(stream_handle) * 1000.0f;

        // Cleanup
        stb_vorbis_close(stream_handle);
        fclose(file_handle);

        return sound_data;
    }

    sound_data sound_loader::parse_wav_sound(const std::string& file_path)
    {
        RT_PROFILE_SECTION("sound_loader::parse_wav_sound");
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
