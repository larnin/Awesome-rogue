#include "projectile.h"

std::default_random_engine Projectile::m_randEngine;

Projectile::Projectile(const Location & pos, Team team, std::weak_ptr<Entity> sender)
    : m_team(team)
    , m_pos(pos)
    , m_destroyable(false)
    , m_killed(false)
    , m_sender(sender)
{

}

bool Projectile::isKilled() const
{
    return m_killed;
}

bool Projectile::isDestoyable() const
{
    return m_destroyable;
}

Location Projectile::getPos() const
{
    return m_pos;
}

Team Projectile::getTeam() const
{
    return m_team;
}

HitBox Projectile::getBox() const
{
    return HitBox();
}
