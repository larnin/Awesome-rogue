#ifndef LISTHOLDER_H
#define LISTHOLDER_H

#include "Systemes/updatable.h"
#include "Projectiles/projectilelist.h"
#include "Entities/entitylist.h"
#include "Particules/particulelist.h"
#include "Utilities/noncopiable.h"

class ListHolder : public Updatable, private NonCopiable
{
public:
    ListHolder() = default;
    ListHolder(ListHolder &&) = default;
    ListHolder & operator =(ListHolder &&) = default;
    virtual ~ListHolder() = default;

    virtual void update(const sf::Time &) override;

    ProjectileList projectiles;
    EntityList entities;
    ParticuleList particules;
};

#endif // LISTHOLDER_H
