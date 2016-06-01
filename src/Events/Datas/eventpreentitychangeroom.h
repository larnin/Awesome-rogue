#ifndef EVENTPREPLAYERCHANGEROOM_H
#define EVENTPREPLAYERCHANGEROOM_H

struct EventPreEntityChangeRoom
{
    EventPreEntityChangeRoom(unsigned int id)
        : entityID(id) {}

    unsigned int entityID;
};

#endif // EVENTPREPLAYERCHANGEROOM_H
