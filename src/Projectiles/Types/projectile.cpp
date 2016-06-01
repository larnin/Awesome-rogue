#include "projectile.h"

Projectile::Projectile(const Location & pos, Team team)
    : m_team(team)
    , m_pos(pos)
    , m_destroyable(false)
    , m_killed(false)
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
