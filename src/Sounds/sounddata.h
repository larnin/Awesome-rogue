#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include <SFML/System/Vector2.hpp>

struct SoundData
{
    SoundData(float _volume, float _pitch = 1)
        : pitch(_pitch)
        , volume(_volume)
    {}

    float pitch;
    float volume;
};

struct MusicData
{
    MusicData(float _volume, float _pitch = 1, bool _loop = false)
        : pitch(_pitch)
        , volume(_volume)
        , loop(_loop)
    {}

    float pitch;
    float volume;
    bool loop;
};

#endif // SOUNDDATA_H
