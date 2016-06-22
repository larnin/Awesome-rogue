#ifndef LISTHOLDER_H
#define LISTHOLDER_H

#include "Systemes/updatable.h"
#include "Projectiles/projectilelist.h"
#include "Entities/entitylist.h"
#include "Particules/particulelist.h"

class ListHolder : public Updatable
{
public:
    ListHolder() = default;
    virtual ~ListHolder() = default;

    virtual void update(const sf::Time &);

    ProjectileList projectiles;
    EntityList entities;
    ParticuleList particules;
};

#endif // LISTHOLDER_H
