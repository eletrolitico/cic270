#include "SoundEngine.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#ifdef WIN32
#include <debugapi.h>
#else
#include <signal.h>
#endif

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
#ifdef WIN32
        DebugBreak();
#else
        raise(SIGTRAP);
#endif
        return false;
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
#ifdef WIN32
        DebugBreak();
#else
        raise(SIGTRAP);
#endif
        return false;
    }
    return true;
}

SoundEngine *SoundEngine::instance = nullptr;

SoundEngine *SoundEngine::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new SoundEngine();
    }
    return instance;
}

SoundEngine::SoundEngine()
{
    m_ALDevice = alcOpenDevice(nullptr);
    if (!m_ALDevice)
    {
        std::cerr << "Failed to open audio device" << std::endl;
#ifdef WIN32
        DebugBreak();
#else
        raise(SIGTRAP);
#endif
    }

    m_ALContext = alcCreateContext(m_ALDevice, nullptr);
    check_alc_errors(m_ALDevice);

    if (!alcMakeContextCurrent(m_ALContext))
    {
        std::cerr << "Error making context current" << std::endl;
#ifdef WIN32
        DebugBreak();
#else
        raise(SIGTRAP);
#endif
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
    for (auto a : m_StreamMap)
    {
        alDeleteSources(1, &a.second.source);
        alDeleteBuffers(NUM_BUFFERS, a.second.buffers);
        stb_vorbis_close(a.second.stream);
    }
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_ALContext);
    alcCloseDevice(m_ALDevice);
}

bool SoundEngine::loadAudio(const std::string &path, const std::string &name, float gain, bool repeat)
{
    int channels, sample_rate;
    short *data;
    int samples = stb_vorbis_decode_filename(path.c_str(), &channels, &sample_rate, &data);

    ALuint buf;
    alGenBuffers(1, &buf);
    check_al_errors();

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
    check_al_errors();

    ALuint source;
    alGenSources(1, &source);
    check_al_errors();
    alSourcef(source, AL_PITCH, 1);
    check_al_errors();
    alSourcef(source, AL_GAIN, gain);
    check_al_errors();
    alSource3f(source, AL_POSITION, 0, 0, 0);
    check_al_errors();
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    check_al_errors();
    alSourcei(source, AL_LOOPING, repeat);
    check_al_errors();
    alSourcei(source, AL_BUFFER, buf);
    check_al_errors();

    m_SMap[name] = {buf, source, repeat};

    if (repeat)
    {
        alSourcePlay(m_SMap[name].source);
        check_al_errors();
    }

    return true;
}

bool SoundEngine::streamAudio(const std::string &path, const std::string &name, float gain, bool repeat)
{
    AudioStream &audio = m_StreamMap[name];
    audio.shouldLoop = repeat;

    int err;
    audio.stream = stb_vorbis_open_filename(path.c_str(), &err, NULL);
    if (!audio.stream)
    {
        std::cerr << "audio"
                  << "\"" << name << "\""
                  << "not found" << std::endl;
        return false;
    }

    alGenBuffers(NUM_BUFFERS, audio.buffers);
    check_al_errors();

    if (audio.stream->channels == 2)
        audio.format = AL_FORMAT_STEREO16;
    else
        audio.format = AL_FORMAT_MONO16;

    alGenSources(1, &audio.source);
    check_al_errors();
    alSourcef(audio.source, AL_GAIN, gain);
    check_al_errors();

    int numBuffers = 0;

    for (int i = 0; i < NUM_BUFFERS; ++i)
        if (fillBuffer(name, audio.buffers[i]))
            numBuffers++;

    if (numBuffers == 0)
        return false;

    alSourceQueueBuffers(audio.source, numBuffers, audio.buffers);
    check_al_errors();

    audio.totalSamplesLeft = stb_vorbis_stream_length_in_samples(audio.stream) * audio.stream->channels;

    if (repeat)
        alSourcePlay(audio.source);
    return true;
}

void SoundEngine::playAudio(const std::string &name)
{
    alSourcePlay(getSource(name));
    check_al_errors();
}

void SoundEngine::stopAudio(const std::string &name)
{
    alSourceStop(getSource(name));
    check_al_errors();
}

void SoundEngine::pauseAudio(const std::string &name)
{
    alSourcePause(getSource(name));
    check_al_errors();
}

bool SoundEngine::fillBuffer(const std::string &name, int buffer)
{
    AudioStream &audio = m_StreamMap[name];
    ALshort pcm[SBUFFER_SIZE];
    int size = 0;
    int result = 0;

    while (size < SBUFFER_SIZE)
    {
        result = stb_vorbis_get_samples_short_interleaved(audio.stream, audio.stream->channels, pcm + size, SBUFFER_SIZE - size);
        if (result > 0)
            size += result * audio.stream->channels;
        else
            break;
    }

    if (size == 0)
        return false;

    alBufferData(buffer, audio.format, pcm, size * sizeof(ALshort), audio.stream->sample_rate);
    check_al_errors();
    audio.totalSamplesLeft -= size;

    return true;
}

bool SoundEngine::streamUpdate(const std::string &name)
{
    AudioStream &audio = m_StreamMap[name];

    ALint processed = 0;

    alGetSourcei(audio.source, AL_BUFFERS_PROCESSED, &processed);
    check_al_errors();

    while (processed--)
    {
        ALuint buffer = 0;

        alSourceUnqueueBuffers(audio.source, 1, &buffer);
        check_al_errors();

        if (not fillBuffer(name, buffer))
        {
            bool shouldExit = true;

            if (audio.shouldLoop)
            {
                stb_vorbis_seek_start(audio.stream);
                audio.totalSamplesLeft = stb_vorbis_stream_length_in_samples(audio.stream) * audio.stream->channels;
                shouldExit = not fillBuffer(name, buffer);
            }

            if (shouldExit)
                return false;
        }
        alSourceQueueBuffers(audio.source, 1, &buffer);
        check_al_errors();
    }

    return true;
}

void SoundEngine::update()
{
    for (auto a : m_StreamMap)
        streamUpdate(a.first);
}

ALuint SoundEngine::getSource(const std::string &name)
{
    auto t = m_SMap.find(name);
    auto t2 = m_StreamMap.find(name);
    if (t == m_SMap.end() && t2 == m_StreamMap.end())
    {
        std::cout << "Attempted to get non existant audio" << std::endl;
        return 0;
    }
    if (t == m_SMap.end())
        return m_StreamMap[name].source;
    else
        return m_SMap[name].source;
}