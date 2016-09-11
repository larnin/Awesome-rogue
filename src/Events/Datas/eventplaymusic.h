#ifndef EVENTPLAYMUSIC_H
#define EVENTPLAYMUSIC_H

#include <string>
#include "Sounds/sounddata.h"

struct EventPlayMusic
{
    EventPlayMusic(const std::string & _filename, const MusicData & _data = MusicData(1), float _fadeout = 0, float _fadein = 0)
        : EventPlayMusic(_filename, _data, _fadeout, _fadein, _fadeout)
    {}

    EventPlayMusic(const std::string & _filename, const MusicData & _data, float _fadeout, float _fadein, float _delay)
        : filename(_filename)
        , data(_data)
        , fadeinTime(_fadein)
        , fadeoutTime(_fadeout)
        , delay(_delay)
    {}

    std::string filename;
    MusicData data;
    float fadeinTime;
    float fadeoutTime;
    float delay;
};

#endif // EVENTPLAYMUSIC_H
