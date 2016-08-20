#ifndef EVENTPLAYMUSIC_H
#define EVENTPLAYMUSIC_H

#include <string>
#include "Sounds/sounddata.h"

struct EventPlayMusic
{
    EventPlayMusic(const std::string & _filename, const MusicData & _data = MusicData(1))
        : filename(_filename)
        , data(_data)
    {}

    std::string filename;
    MusicData data;
};

#endif // EVENTPLAYMUSIC_H
