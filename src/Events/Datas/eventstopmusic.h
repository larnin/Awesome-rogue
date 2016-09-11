#ifndef EVENTSTOPMUSIC_H
#define EVENTSTOPMUSIC_H

struct EventStopMusic
{
    EventStopMusic(float _fadeout = 0)
        : fadeoutTime(_fadeout)
    {}

    float fadeoutTime;
};

#endif // EVENTSTOPMUSIC_H
