#ifndef EVENTDROPITEM_H
#define EVENTDROPITEM_H

#include "Map/location.h"
#include "Items/itemtype.h"

struct EventDropItem
{
    EventDropItem(ItemType _type, const Location & _pos, const sf::Vector2f & _speed)
        : speed(_speed), pos(_pos), type(_type) {}

    sf::Vector2f speed;
    Location pos;
    ItemType type;
};

#endif // EVENTDROPITEM_H
