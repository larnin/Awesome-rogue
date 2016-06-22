#include "particule.h"

std::default_random_engine Particule::m_randEngine;

Particule::Particule(const Location & pos)
    : m_pos(pos)
    , m_killed(false)
{

}

bool Particule::isKilled() const
{
    return m_killed;
}

Location Particule::getPos() const
{
    return m_pos;
}
