#ifndef EVENTITEMLOADED_H
#define EVENTITEMLOADED_H

#include "File/serializable.h"
#include "File/serializabletype.h"
#include <memory>

struct EventItemLoaded
{
    EventItemLoaded(SerializableType _type, std::shared_ptr<Serializable> _item)
        : type(_type), item(_item)
    {}

    SerializableType type;
    std::shared_ptr<Serializable> item;
};

#endif // EVENTITEMLOADED_H
