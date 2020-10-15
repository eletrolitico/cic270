#include "SoundEngine.h"
#include <iostream>
#include <signal.h>

#include "stb_vorbis.c"

bool check_al_errors()
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        //std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error)
        {
        case AL_INVALID_NAME:
            std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            break;
        default:
            std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
        raise(SIGTRAP);
    }
    return true;
}

bool check_alc_errors(ALCdevice *device)
{
    ALCenum error = alcGetError(device);
    if (error != ALC_NO_ERROR)
    {
        //std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error)
        {
        case ALC_INVALID_VALUE:
            std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case ALC_INVALID_DEVICE:
            std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
            break;
        case ALC_INVALID_CONTEXT:
            std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
            break;
        case ALC_INVALID_ENUM:
            std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
            break;
        case ALC_OUT_OF_MEMORY:
            std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
            break;
        default:
            std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
        raise(SIGTRAP);
    }
    return true;
}

SoundEngine::SoundEngine()
{
    m_ALDevice = alcOpenDevice(nullptr);
    if (!m_ALDevice)
    {
        std::cerr << "Failed to open audio device" << std::endl;
        raise(SIGTRAP);
    }

    m_ALContext = alcCreateContext(m_ALDevice, nullptr);
    check_alc_errors(m_ALDevice);

    if (!alcMakeContextCurrent(m_ALContext))
    {
        std::cerr << "Error making context current" << std::endl;
        raise(SIGTRAP);
    }
    check_alc_errors(m_ALDevice);
}

SoundEngine::~SoundEngine()
{
    for (auto a : m_SMap)
    {
        alDeleteSources(1, &a.second.source);
        alDeleteBuffers(1, &a.second.buffer);
    }
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_ALContext);
    alcCloseDevice(m_ALDevice);
}

bool SoundEngine::loadAudio(const std::string &path, const std::string &name, float gain)
{
    int channels, sample_rate;
    short *data;
    int samples = stb_vorbis_decode_filename(path.c_str(), &channels, &sample_rate, &data);

    ALuint buf;
    alGenBuffers(1, &buf);

    ALenum format;
    if (channels == 1)
        format = AL_FORMAT_MONO16;
    else if (channels == 2)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr << "ERROR: unrecognised wave format: " << channels << " channels, " << std::endl;
        return false;
    }

    int size = samples * sizeof(short);
    if (channels == 2)
        size *= 2;

    alBufferData(buf, format, data, size, sample_rate);

    ALuint source;
    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, gain);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcei(source, AL_BUFFER, buf);

    m_SMap[name] = {buf, source, false};

    return true;
}

void SoundEngine::playAudio(const std::string &name, bool repeat)
{
    alSourcePlay(m_SMap[name].source);
    m_SMap[name].repeat = repeat;
}

void SoundEngine::update()
{
    ALint state;
    for (auto a : m_SMap)
    {
        alGetSourcei(a.second.source, AL_SOURCE_STATE, &state);
        if (a.second.repeat && state != AL_PLAYING)
            alSourcePlay(a.second.source);
    }
}