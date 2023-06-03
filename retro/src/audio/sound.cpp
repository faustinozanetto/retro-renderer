#include "rtpch.h"
#include "sound.h"

#include <stb_vorbis.c>
#include <AL/al.h>

#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"

namespace retro::audio
{
    sound::sound(const std::string& file_path)
    {
        RT_TRACE("Retro Renderer | Started loading sound from file.");
        RT_TRACE("    - File Name: {0}", file_path.c_str());
        m_file_path = file_path;
        m_sound_data = load_sound_from_file();
        RT_TRACE("    - Frequency: {0}", m_sound_data.frequency);
        RT_TRACE("    - Lenght: {0}", m_sound_data.length);
        RT_TRACE("    - Bit Rate: {0}", m_sound_data.bit_rate);
        RT_TRACE("    - Size: {0}", m_sound_data.size);
        RT_TRACE("    - Channels: {0}", m_sound_data.channels);

        // Generate openal buffers
        alGenBuffers(1, &m_sound_buffer);
        alBufferData(m_sound_buffer, AL_FORMAT_STEREO16, m_sound_data.data,
                     m_sound_data.size,
                     static_cast<ALsizei>(m_sound_data.frequency));
        RT_TRACE("Retro Renderer | Sound loaded successfully!");
    }

    sound::~sound()
    {
        alDeleteBuffers(1, &m_sound_buffer);
        delete m_sound_data.data;
    }

    sound_data sound::load_sound_from_file() const
    {
        sound_data data{};

        const std::string extension = get_file_extension(m_file_path);
        if (extension == ".ogg")
        {
            FILE* file_handle = fopen(m_file_path.c_str(), "rb");
            if (!file_handle)
            {
                RT_ASSERT_MSG(false, "Could not open sound file: {0}", m_file_path.c_str());
            }

            int error_code;
            stb_vorbis* stream_handle =
                stb_vorbis_open_filename(m_file_path.c_str(), &error_code, nullptr);
            if (!stream_handle)
            {
                RT_ASSERT_MSG(false, "Could not read sound file: {0}", m_file_path.c_str());
            }

            const stb_vorbis_info sound_information = stb_vorbis_get_info(stream_handle);
            data.channels = sound_information.channels;
            data.bit_rate = 16;
            data.frequency = sound_information.sample_rate;
            const uint32_t size = stb_vorbis_stream_length_in_samples(stream_handle) *
                data.channels * sizeof(int16_t);
            data.size = size;
            data.data = new unsigned char[data.size];

            stb_vorbis_get_samples_short_interleaved(
                stream_handle, data.channels,
                reinterpret_cast<short*>(data.data), data.size);
            data.length = stb_vorbis_stream_length_in_seconds(stream_handle) * 1000.0f;

            // Cleanup
            stb_vorbis_close(stream_handle);
            fclose(file_handle);
        }
        else if (extension == ".wav")
        {
            // Load the sound data from the file
            unsigned int channels;
            unsigned int sampleRate;
            drwav_uint64 totalPCMFrameCount;
            float* pSampleData = drwav_open_file_and_read_pcm_frames_f32(
                m_file_path.c_str(), &channels, &sampleRate, &totalPCMFrameCount, NULL);
            if (!pSampleData)
            {
                RT_ASSERT_MSG(false, "Could not open sound file: {0}", m_file_path.c_str());
            }

            // Extract the sound data information
            data.frequency = static_cast<float>(sampleRate);
            data.bit_rate = 16;
            data.channels = channels;

            // Read the sound data into memory
            data.size = static_cast<uint32_t>(totalPCMFrameCount * channels * sizeof(float));
            data.data = new unsigned char[data.size];
            if (!data.data)
            {
                drwav_free(pSampleData, NULL);
                RT_ASSERT_MSG(false, "Could not allocate sound data!");
            }

            std::memcpy(data.data, pSampleData, data.size);

            // Calculate the length of the sound in seconds
            data.length = static_cast<double>(totalPCMFrameCount) / static_cast<double>(sampleRate);

            // Clean up
            drwav_free(pSampleData, NULL);
        }
        else if (extension == ".mp3")
        {
            drmp3 mp3;
            if (!drmp3_init_file(&mp3, m_file_path.c_str(), NULL))
            {
                RT_ASSERT_MSG(false, "Could not open sound file: {0}", m_file_path.c_str());
            }

            /*
            drmp3_uint64 framesRead = drmp3_read_pcm_frames_f32(mp3, framesToRead, pFrames);
            
            data.channels = mp3.channels;
            data.bit_rate = 16;
            data.frequency = mp3.sampleRate;
            
            uint32_t size = stb_vorbis_stream_length_in_samples(stream_handle) *
                data.channels * sizeof(int16_t);
            data.size = size;
            data.data = new unsigned char[data.size];
            */
        }

        RT_ASSERT_MSG(data.data, "Failed to create sound data!");
        return data;
    }

    std::string sound::get_file_extension(const std::string& file_path) const
    {
        size_t dotIndex = file_path.find_last_of('.');
        if (dotIndex != std::string::npos)
            return file_path.substr(dotIndex);

        return "";
    }
}
