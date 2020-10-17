#ifndef SOUND_ENGINE
#define SOUND_ENGINE

#include <unordered_map>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>

#include "stb_vorbis.c"

#ifndef NUM_BUFFERS
#define NUM_BUFFERS 4
#endif

#ifndef SBUFFER_SIZE
#define SBUFFER_SIZE 1024 * 32

typedef struct
{
    ALuint buffer;
    ALuint source;
    bool repeat;
} AudioSrc;

typedef struct
{
    ALuint ID;

    stb_vorbis *stream;
    stb_vorbis_info info;

    ALuint buffers[NUM_BUFFERS];
    ALuint source;
    ALenum format;

    size_t totalSamplesLeft;

    bool shouldLoop;
} AudioStream;
#endif

class SoundEngine
{
public:
    SoundEngine();
    ~SoundEngine();
    bool loadAudio(const std::string &path, const std::string &name, float gain = 1.0f, bool repeat = false);
    void playAudio(const std::string &name);
    void stopAudio(const std::string &name);
    bool streamAudio(const std::string &path, const std::string &name, float gain = 1.0f, bool repeat = true);
    void update();

private:
    ALCdevice *m_ALDevice;
    ALCcontext *m_ALContext;
    std::unordered_map<std::string, AudioSrc> m_SMap;
    std::unordered_map<std::string, AudioStream> m_StreamMap;

    bool fillBuffer(const std::string &name, int buffer);
    bool streamUpdate(const std::string &name);
};

#endif