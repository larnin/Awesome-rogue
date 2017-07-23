#ifndef EVENTENTITYCHANGEROOM_H
#define EVENTENTITYCHANGEROOM_H

struct EventEntityChangeRoom
{
public:
    EventEntityChangeRoom(unsigned int id)
        : entityID(id) {}

    unsigned int entityID;
};

#endif // EVENTENTITYCHANGEROOM_H
