#ifndef EVENTITEMLOADED_H
#define EVENTITEMLOADED_H

#include "File/serializable.h"
#include "File/serializabletype.h"
#include <memory>

template <typename T>
struct EventItemLoaded
{
    EventItemLoaded(std::shared_ptr<T> _item)
        : item(_item)
    {}

    std::shared_ptr<T> item;
};

#endif // EVENTITEMLOADED_H
