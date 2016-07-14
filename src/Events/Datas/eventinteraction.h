#ifndef EVENTINTERACTION_H
#define EVENTINTERACTION_H

#include "Map/blocktype.h"
#include "Map/location.h"

struct EventInteraction
{
    EventInteraction(const Location & _pos, BlockInteractionType _type)
        : pos(_pos)
        , type(_type)
    {

    }

    Location pos;
    BlockInteractionType type;
};

#endif // EVENTINTERACTION_H
