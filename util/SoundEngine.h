#ifndef SOUND_ENGINE
#define SOUND_ENGINE

#include <unordered_map>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>

typedef struct
{
    ALuint buffer;
    ALuint source;
    bool repeat;
} AudioSrc;

class SoundEngine
{
public:
    SoundEngine();
    ~SoundEngine();
    bool loadAudio(const std::string &path, const std::string &name, float gain = 1.0f);
    void playAudio(const std::string &name, bool repeat = false);
    void update();

private:
    ALCdevice *m_ALDevice;
    ALCcontext *m_ALContext;
    std::unordered_map<std::string, AudioSrc> m_SMap;
};

#endif