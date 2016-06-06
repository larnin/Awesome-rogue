#ifndef EVENTPLAYERCHANGEROOM_H
#define EVENTPLAYERCHANGEROOM_H

struct EventPlayerChangeRoom
{
    EventPlayerChangeRoom(unsigned int id)
        : entityID(id) {}

    unsigned int entityID;
};

#endif // EVENTPLAYERCHANGEROOM_H
