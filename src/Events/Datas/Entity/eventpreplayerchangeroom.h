#ifndef EVENTPREPLAYERCHANGEROOM_H
#define EVENTPREPLAYERCHANGEROOM_H

struct EventPrePlayerChangeRoom
{
    EventPrePlayerChangeRoom(unsigned int id)
        : entityID(id) {}

    unsigned int entityID;
};

#endif // EVENTPREPLAYERCHANGEROOM_H
