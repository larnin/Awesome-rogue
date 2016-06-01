#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Systemes/updatable.h"
#include "Utilities/noncopiable.h"
#include "Entities/Types/entity.h"
#include "Map/location.h"
#include <SFML/Graphics/Drawable.hpp>

class Projectile : public sf::Drawable, public Updatable, public NonCopiable
{
public:
    Projectile(const Location & pos, Team team);
    Projectile(Projectile&&) = default;
    Projectile & operator= (Projectile &&) = default;
    virtual ~Projectile() = default;

    bool isKilled() const;
    bool isDestoyable() const;
    Location getPos() const;
    Team getTeam() const;


protected:
    Team m_team;
    Location m_pos;
    bool m_destroyable;
    bool m_killed;
};

#endif // PROJECTILE_H
