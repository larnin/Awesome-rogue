#ifndef EVENTENTITYCREATED_H
#define EVENTENTITYCREATED_H

#include "Entities/Types/entity.h"
#include <memory>

struct EventEntityCreated
{
    EventEntityCreated(std::shared_ptr<Entity> _e)
        : entity(_e) {}

    std::shared_ptr<Entity> entity;
};

#endif // EVENTENTITYCREATED_H
