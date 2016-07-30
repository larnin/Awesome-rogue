#ifndef EVENTREMOVEENTITY_H
#define EVENTREMOVEENTITY_H

#include <memory>
#include "Entities/Types/entity.h"

struct EventRemoveEntity
{
    EventRemoveEntity(std::shared_ptr<Entity> _e)
        : entity(_e) {}

    std::shared_ptr<Entity> entity;
};

#endif // EVENTREMOVEENTITY_H
