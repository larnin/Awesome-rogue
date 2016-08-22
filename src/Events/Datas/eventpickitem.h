#ifndef EVENTPICKITEM_H
#define EVENTPICKITEM_H

#include "Items/itemtype.h"

struct EventPickItem
{
    EventPickItem(ItemType _type)
        : type(_type) {}

    ItemType type;
};

#endif // EVENTPICKITEM_H
