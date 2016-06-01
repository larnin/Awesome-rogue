#ifndef EVENTPROJECTILECREATED_H
#define EVENTPROJECTILECREATED_H

#include "Projectiles/Types/projectile.h"

struct EventProjectileCreated
{
    EventProjectileCreated(std::shared_ptr<Projectile> p)
        : projectile(p)
    {}

    std::shared_ptr<Projectile> projectile;
};

#endif // EVENTPROJECTILECREATED_H
