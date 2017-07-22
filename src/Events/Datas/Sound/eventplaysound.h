#ifndef EVENTPLAYSOUND_H
#define EVENTPLAYSOUND_H

#include "Sounds/sounddata.h"
#include <string>

struct EventPlaySound
{
    EventPlaySound(const std::string & _filename, const SoundData & _data = SoundData(1))
        : filename(_filename)
        , data(_data)
    {}

    std::string filename;
    SoundData data;
};

#endif // EVENTPLAYSOUND_H
