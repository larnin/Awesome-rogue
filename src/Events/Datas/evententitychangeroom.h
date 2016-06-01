#ifndef EVENTENTITYCHANGEROOM_H
#define EVENTENTITYCHANGEROOM_H

struct EventEntityChangeRoom
{
    EventEntityChangeRoom(unsigned int id)
        : entityID(id) {}

    unsigned int entityID;
};

#endif // EVENTENTITYCHANGEROOM_H
