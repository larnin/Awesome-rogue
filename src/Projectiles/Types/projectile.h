#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <random>
#include "Systemes/updatable.h"
#include "Utilities/noncopiable.h"
#include "Map/location.h"
#include <SFML/Graphics/Drawable.hpp>
#include <memory>
#include "Entities/team.h"

class Entity;
class HitBox;

class Projectile : public sf::Drawable, public Updatable, private NonCopiable
{
public:
    Projectile(const Location & pos, Team team, std::weak_ptr<Entity> sender);
    Projectile(Projectile&&) = default;
    Projectile & operator=(Projectile &&) = default;
    virtual ~Projectile() = default;

    bool isKilled() const;
    bool isDestoyable() const;
    Location getPos() const;
    Team getTeam() const;


protected:
    virtual HitBox getBox() const;

    Team m_team;
    Location m_pos;
    bool m_destroyable;
    bool m_killed;
    std::weak_ptr<Entity> m_sender;

    static std::default_random_engine m_randEngine;
};

#endif // PROJECTILE_H
