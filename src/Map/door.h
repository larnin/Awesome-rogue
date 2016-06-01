#ifndef DOOR_H
#define DOOR_H

#include <memory>
#include "location.h"
#include "orientation.h"

class Room;

struct Door
{
public:
    Door(const Location & _pos = Location(), const Location & _dest = Location());
    ~Door() = default;

    bool isValid() const;
    Orientation getOrientation() const;

    Location pos;
    Location dest;
};

#endif // DOOR_H
